#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <thread>
#include <vector>
#include "types.h"
#include "Protocol.h"

namespace OAO
{
	class ClientSession;

	class BaseServer
	{
	public:
		BaseServer() : mCompletionPort(0), mListenSocket(0) {}
		virtual ~BaseServer() = default;

		result Initialize();
		result Start();
		result Listen();
		void Shutdown();

		bool CreateAccepterThread();
		void AccepterThread();

		bool CreateWorkerThread();
		void WorkerThread();

		virtual void Init();
		virtual void OnConnect(const UINT32 clientIndex);
		virtual void OnClose(const UINT32 clientIndex);
		virtual void OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData);

		void CloseSocket(ClientSession* clientSession, bool isForce = false);

		HANDLE GetCompletionPort() { return mCompletionPort; }

		SOCKET* GetListenSocket() { return &mListenSocket; }

	public:
		static char mAcceptBuf[64];
		static LPFN_ACCEPTEX mFnAcceptEx;
		static LPFN_CONNECTEX mFnConnectEx;
	protected:

	private:
		HANDLE mCompletionPort;
		SOCKET mListenSocket;

		std::vector<std::thread> mIOWorkerThreads;
		std::thread mAccepterThread;

		bool isWorkerRun = false;
		bool isAccepterRun = false;

	};

	BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);

	BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength,
		LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);
}