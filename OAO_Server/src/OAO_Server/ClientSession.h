#pragma once
#include <winsock2.h>
#include <mutex>
#include <queue>
#include "types.h"
#include "Protocol.h"

namespace OAO
{
	class ClientSession
	{
	public:
		ClientSession();

		void Initialize(const UINT32 index, HANDLE iocpHandle);

		bool IsConnected();

		UINT32 GetIndex();
		SOCKET GetSocket();
		UINT64 GetLastestClosedTimeSec();

		char* RecvBuffer();

		bool OnConnect(HANDLE iocpHandle, SOCKET socket);

		void Close(bool isForce = false);

		bool PostAccept(SOCKET listenSock);
		bool AcceptCompletion();

		bool BindIOCompletionPort(HANDLE iocpHandle);

		bool BindRecv();

		bool SendMsg(const UINT32 dataSize, char* pMsg);

		void SendCompleted(const UINT32 dataSize);

	private:
		bool SendIO();

		bool SetSocketOption();

	private:
		INT32 mIndex = 0;
		HANDLE mIOCPHandle = INVALID_HANDLE_VALUE;

		INT64 mIsConnect = 0;
		UINT64 mLastestClosedTimeSec = 0;

		SOCKET mSocket;

		stOverlappedEx mAcceptContext;
		char mAcceptBuf[64];

		stOverlappedEx mRecvOverlappedEx;
		char mRecvBuf[MAX_BUF_SIZE];

		std::mutex mSendLock;
		std::queue<stOverlappedEx*> mSendDataQueue;
	};
}