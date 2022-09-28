#include "BaseServer.h"
<<<<<<< HEAD

namespace OAO
{
	BaseServer::BaseServer(server_type serverType) :
		mServerType(serverType)
	{
	}

	auto BaseServer::Initialize() -> result
	{

		return result::success;
	}
	auto BaseServer::Shutdown() -> void
	{
	}
=======
#include <process.h>
#include <iostream>
#include "types.h"
#include "ConnectionManager.h"

namespace OAO
{
	LPFN_ACCEPTEX BaseServer::mFnAcceptEx = nullptr;
	LPFN_CONNECTEX BaseServer::mFnConnectEx = nullptr;
	char BaseServer::mAcceptBuf[64] = { 0, };

	BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
	{
		return BaseServer::mFnAcceptEx(sListenSocket, sAcceptSocket, lpOutputBuffer, dwReceiveDataLength,
			dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped);
	}

	BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
	{
		return BaseServer::mFnConnectEx(hSocket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped);
	}

	result BaseServer::Initialize()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cout << "[DEBUG] WSAStartup error : " << WSAGetLastError() << std::endl;
			return result::failure;
		}

		mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (mListenSocket == INVALID_SOCKET)
		{
			std::cout << "[DEBUG] Create TCP Socket error : " << WSAGetLastError() << std::endl;
			return result::failure;
		}
		return result::success;
	}

	result BaseServer::Start()
	{
		ConnectionManager::get_instance().PrepareClientSessions(mCompletionPort);

		bool bRet = CreateWorkerThread();
		if (bRet == false) {
			return result::failure;
		}

		bRet = CreateAccepterThread();
		if (bRet == false) {
			return result::failure;
		}

		std::cout << "서버 시작" << std::endl;

		return result::success;
	}

	result BaseServer::Listen()
	{
		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(LISTEN_PORT);
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		if (SOCKET_ERROR == bind(mListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
		{
			std::cout << "[DEBUG] Listen Socket Bind error : " << GetLastError() << std::endl;
			return result::failure;
		}

		if (SOCKET_ERROR == listen(mListenSocket, SOMAXCONN))
		{
			std::cout << "[DEBUG] Listen error : " << GetLastError() << std::endl;
			return result::failure;
		}

		mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (mCompletionPort == INVALID_HANDLE_VALUE)
		{
			std::cout << "[DEBUG] CreateIOCompletionPort error : " << WSAGetLastError() << std::endl;
			return result::failure;
		}

		HANDLE handle = CreateIoCompletionPort((HANDLE)mListenSocket, mCompletionPort, 0, 0);
		if (handle != mCompletionPort)
		{
			std::cout << "[DEBUG] Listen Socket IOCP Register error : " << GetLastError() << std::endl;
		}

		DWORD bytes = 0;

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		if (SOCKET_ERROR == WSAIoctl(mListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &mFnAcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL)) {
			return result::failure;
		}

		GUID guidConnectEx = WSAID_CONNECTEX;
		if (SOCKET_ERROR == WSAIoctl(mListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx, sizeof(GUID), &mFnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL)) {
			return result::failure;
		}

		std::cout << "Bind Success!" << std::endl;
		return result::success;
	}

	void BaseServer::Shutdown()
	{
	}

	bool BaseServer::CreateAccepterThread()
	{
		isAccepterRun = true;
		mAccepterThread = std::thread([this]() {AccepterThread(); });

		std::cout << "AccepterThread 시작.." << std::endl;
		return true;
	}
	void BaseServer::AccepterThread()
	{
		while (isAccepterRun)
		{
			auto curTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

			//for (auto client : ConnectionManager::get_instance().mSessionList)
			//{
			//	if (client->IsConnected())
			//	{
			//		continue;
			//	}

			//	if ((UINT64)curTimeSec < client->GetLastestClosedTimeSec())
			//	{
			//		continue;
			//	}

			//	auto diff = curTimeSec - client->GetLastestClosedTimeSec();
			//	if (diff <= RE_USE_SESSION_WAIT_TIMESEC)
			//	{
			//		continue;
			//	}
			//	client->PostAccept(mListenSocket);
			//}

			ConnectionManager::get_instance().AcceptClientSessions(mListenSocket, curTimeSec);

			std::this_thread::sleep_for(std::chrono::milliseconds(32));
		}
	}

	bool BaseServer::CreateWorkerThread()
	{
		auto threadCount = (4 * 2) + 1;
		for (UINT32 i = 0; i < threadCount; i++)
		{
			mIOWorkerThreads.emplace_back([this]() {WorkerThread(); });
		}
		isWorkerRun = true;
		std::cout << "워커쓰레드 시작" << std::endl;
		return true;
	}

	void BaseServer::WorkerThread()
	{
		ClientSession* pClientSession = nullptr;
		BOOL bSuccess = TRUE;
		DWORD dwIOSize = 0;
		LPOVERLAPPED lpOverlapped = NULL;

		while (isWorkerRun)
		{
			bSuccess = GetQueuedCompletionStatus(mCompletionPort,
				&dwIOSize,					// 실제로 전송된 바이트
				(PULONG_PTR)&pClientSession,		// CompletionKey
				&lpOverlapped,				// Overlapped IO 객체
				INFINITE);					// 대기할 시간

			//사용자 쓰레드 종료 메세지 처리..
			if (TRUE == bSuccess && 0 == dwIOSize && NULL == lpOverlapped)
			{
				isWorkerRun = false;
				continue;
			}

			if (NULL == lpOverlapped)
			{
				continue;
			}

			auto pOverlappedEx = (stOverlappedEx*)lpOverlapped;

			//client가 접속을 끊었을때..			
			if (FALSE == bSuccess || (0 == dwIOSize && IOOp::ACCEPT != pOverlappedEx->mOperation))
			{
				printf("socket(%d) 접속 끊김\n", (int)pClientSession->GetSocket());
				ConnectionManager::get_instance().ReturnClientSession(*pClientSession);
				CloseSocket(pClientSession); //Caller WokerThread()
				continue;
			}


			if (IOOp::ACCEPT == pOverlappedEx->mOperation)
			{
				pClientSession = ConnectionManager::get_instance().GetClientSession(pOverlappedEx->SessionIndex);
				if (pClientSession->AcceptCompletion())
				{
					OnConnect(pClientSession->GetIndex());
				}
				else
				{
					ConnectionManager::get_instance().ReturnClientSession(*pClientSession);
					CloseSocket(pClientSession, true);  //Caller WokerThread()
				}
			}
			//Overlapped I/O Recv작업 결과 뒤 처리
			else if (IOOp::RECV == pOverlappedEx->mOperation)
			{
				OnReceive(pClientSession->GetIndex(), dwIOSize, pClientSession->RecvBuffer());

				pClientSession->BindRecv();
			}
			//Overlapped I/O Send작업 결과 뒤 처리
			else if (IOOp::SEND == pOverlappedEx->mOperation)
			{
				pClientSession->SendCompleted(dwIOSize);
			}
			//예외 상황
			else
			{
				printf("Client Index(%d)에서 예외상황\n", pClientSession->GetIndex());
			}
		}
	}

	void BaseServer::Init()
	{
	}

	void BaseServer::OnConnect(const UINT32 clinetIndex)
	{
	}

	void BaseServer::OnClose(const UINT32 clinetIndex)
	{
	}

	void BaseServer::OnReceive(const UINT32 clinetIndex, const UINT32 size, char* pData)
	{
	}

	void BaseServer::CloseSocket(ClientSession* clientSession, bool isForce)
	{
		if (clientSession->IsConnected() == false)
		{
			return;
		}

		auto clientIndex = clientSession->GetIndex();

		clientSession->Close(isForce);

		OnClose(clientIndex);
	}
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
}