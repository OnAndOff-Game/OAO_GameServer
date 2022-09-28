#include "ClientSession.h"
#include "BaseServer.h"
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>

OAO::ClientSession::ClientSession()
{
	ZeroMemory(&mRecvOverlappedEx, sizeof(stOverlappedEx));
	mSocket = INVALID_SOCKET;
}

void OAO::ClientSession::Initialize(const UINT32 index, HANDLE iocpHandle)
{
	mIndex = index;
	mIOCPHandle = iocpHandle;
}

bool OAO::ClientSession::IsConnected()
{
	return mIsConnect == 1;
}

UINT32 OAO::ClientSession::GetIndex()
{
	return mIndex;
}

SOCKET OAO::ClientSession::GetSocket()
{
	return mSocket;
}

UINT64 OAO::ClientSession::GetLastestClosedTimeSec()
{
	return mLastestClosedTimeSec;
}

char* OAO::ClientSession::RecvBuffer()
{
	return mRecvBuf;
}

bool OAO::ClientSession::OnConnect(HANDLE iocpHandle, SOCKET socket)
{
	mSocket = socket;
	mIsConnect = 1;

	if (BindIOCompletionPort(iocpHandle) == false)
	{
		return false;
	}

	return BindRecv();
}

void OAO::ClientSession::Close(bool isForce)
{
	struct linger stLinger = { 0,0 };

	if (true == isForce)
	{
		stLinger.l_onoff = 1;
	}

	shutdown(mSocket, SD_BOTH);

	setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	mIsConnect = 0;
	mLastestClosedTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	closesocket(mSocket);
	mSocket = INVALID_SOCKET;
}

bool OAO::ClientSession::PostAccept(SOCKET listenSock)
{
	std::cout << "PostAccept. clinet Index : " << GetIndex() << std::endl;

	mLastestClosedTimeSec = UINT32_MAX;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == mSocket)
	{
		std::cout << "Client Socket WSASocket error : " << GetLastError() << std::endl;
		return false;
	}

	ZeroMemory(&mAcceptContext, sizeof(stOverlappedEx));

	DWORD bytes = 0;
	DWORD flags = 0;
	mAcceptContext.mWsaBuf.len = 0;
	mAcceptContext.mWsaBuf.buf = nullptr;
	mAcceptContext.mOperation = IOOp::ACCEPT;
	mAcceptContext.SessionIndex = mIndex;

	if (FALSE == OAO::AcceptEx(listenSock, mSocket, mAcceptBuf, 0,
		sizeof(SOCKADDR_IN) * 16, sizeof(SOCKADDR_IN) * 16, &bytes, (LPWSAOVERLAPPED) & (mAcceptContext)))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{

			std::cout << "AcceptEx Error : " << GetLastError() << std::endl;
			return false;
		}
	}
	return true;
}

bool OAO::ClientSession::AcceptCompletion()
{
	std::cout << "AcceptCompletion : SessionIndex(" << mIndex << ")" << std::endl;

	if (OnConnect(mIOCPHandle, mSocket) == false)
	{
		return false;
	}

	SOCKADDR_IN stClientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	char clientIP[32] = { 0, };
	
	inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
	std::cout << "클라이언트 접속 : IP(" << clientIP << ") " << "SOCKET(" << (int)mSocket << ")" << std::endl;

	return true;
}

bool OAO::ClientSession::BindIOCompletionPort(HANDLE iocpHandle)
{
	//socket과 pClientInfo를 CompletionPort객체와 연결시킨다.
	auto hIOCP = CreateIoCompletionPort((HANDLE)GetSocket()
		, iocpHandle
		, (ULONG_PTR)(this), 0);

	if (hIOCP == INVALID_HANDLE_VALUE)
	{
		printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
		return false;
	}

	return true;
}

bool OAO::ClientSession::BindRecv()
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	//Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	mRecvOverlappedEx.mWsaBuf.len = MAX_BUF_SIZE;
	mRecvOverlappedEx.mWsaBuf.buf = mRecvBuf;
	mRecvOverlappedEx.mOperation = IOOp::RECV;

	int nRet = WSARecv(mSocket,
		&(mRecvOverlappedEx.mWsaBuf),
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (mRecvOverlappedEx),
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool OAO::ClientSession::SendMsg(const UINT32 dataSize, char* pMsg)
{
	auto sendOverlappedEx = new stOverlappedEx;
	ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));
	sendOverlappedEx->mWsaBuf.len = dataSize;
	sendOverlappedEx->mWsaBuf.buf = new char[dataSize];
	CopyMemory(sendOverlappedEx->mWsaBuf.buf, pMsg, dataSize);
	sendOverlappedEx->mOperation = IOOp::SEND;

	std::lock_guard<std::mutex> guard(mSendLock);

	mSendDataQueue.push(sendOverlappedEx);

	if (mSendDataQueue.size() == 1)
	{
		SendIO();
	}

	return true;
}

void OAO::ClientSession::SendCompleted(const UINT32 dataSize)
{
	printf("[송신 완료] bytes : %d\n", dataSize);

	std::lock_guard<std::mutex> guard(mSendLock);

	delete[] mSendDataQueue.front()->mWsaBuf.buf;
	delete mSendDataQueue.front();

	mSendDataQueue.pop();

	if (mSendDataQueue.empty() == false)
	{
		SendIO();
	}
}

bool OAO::ClientSession::SendIO()
{
	auto sendOverlappedEx = mSendDataQueue.front();

	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(mSocket,
		&(sendOverlappedEx->mWsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED)sendOverlappedEx,
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool OAO::ClientSession::SetSocketOption()
{
	int opt = 1;
	if (SOCKET_ERROR == setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
		return false;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
		return false;
	}

	return true;
}
