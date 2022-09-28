#pragma once
<<<<<<< HEAD

#include <vector>
#include <unordered_set>
#include <memory>
#include <thread>

using namespace std;
=======
#include <list>
#include <vector>
#include <mutex>
#include "types.h"
#include "ClientSession.h"
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90

namespace OAO
{
	class ConnectionManager
	{
	public:
<<<<<<< HEAD

	private:
=======
		static auto get_instance() -> ConnectionManager& {
			static ConnectionManager s_singleton;
			return s_singleton;
		}

	public:
		ConnectionManager();
		~ConnectionManager();

		void PrepareClientSessions(HANDLE iocpHandle);
		bool AcceptClientSessions(SOCKET listenSocket, UINT64 curTime);
		void ReturnClientSession(ClientSession& client);
		int MaxClientSessionCount();

		ClientSession* GetClientSession(const int index);

	private:
		typedef std::list<ClientSession*> ClientList;
		ClientList mFreeSessionList;
		std::vector<ClientSession*> mSessionList;
		uint64_t mCurrentIssueCount = 0;
		uint64_t mCurrentReturnCount = 0;
		int mMaxSessionCount = 0;

		std::mutex mLock;
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
	};
}