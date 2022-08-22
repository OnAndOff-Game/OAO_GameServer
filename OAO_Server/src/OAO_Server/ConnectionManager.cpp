#include "ConnectionManager.h"
#include "ClientSession.h"
#include "Protocol.h"

#include <iostream>

namespace OAO
{
	ConnectionManager::ConnectionManager()
	{
	}

	ConnectionManager::~ConnectionManager()
	{
		for (auto it : mFreeSessionList)
		{
			delete it;
		}
	}

	void ConnectionManager::PrepareClientSessions(HANDLE iocpHandle)
	{
		mMaxSessionCount = MAX_USER_COUNT;

		for (int i = 0; i < mMaxSessionCount; i++)
		{
			ClientSession* client = new ClientSession();
			client->Initialize(i, iocpHandle);

			mFreeSessionList.push_back(client);
			mSessionList.push_back(client);
		}

		std::cout << "[DEBUG] MaxSessionCount : " << mMaxSessionCount << std::endl;
	}

	bool ConnectionManager::AcceptClientSessions(SOCKET listenSocket, UINT64 curTime)
	{
		std::lock_guard<std::mutex> guard(mLock);
		while (mCurrentIssueCount - mCurrentReturnCount < mMaxSessionCount)
		{
			ClientSession* newClient = mFreeSessionList.back();
			mFreeSessionList.pop_back();

			++mCurrentIssueCount;

			if (false == newClient->PostAccept(listenSocket)) {
				return false;
			}
		}

		return true;
	}

	void ConnectionManager::ReturnClientSession(ClientSession& client)
	{
		std::lock_guard<std::mutex> guard(mLock);

		if (!client.IsConnected()) return;

		mFreeSessionList.push_back(&client);

		++mCurrentReturnCount;
	}

	int ConnectionManager::MaxClientSessionCount()
	{
		return 0;
	}

	ClientSession* ConnectionManager::GetClientSession(const int index)
	{
		if (index < 0 || index >= mMaxSessionCount) {
			return nullptr;
		}

		return mSessionList[index];
	}
}
