#pragma once
#include "OAO_Server/BaseServer.h"
using namespace OAO;

class GameServer final : public BaseServer
{
public:
	static auto get_instance() -> GameServer& {
		static GameServer s_singleton;
		return s_singleton;
	}
	virtual void Init() override;
	virtual void OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData) override;
	void Chat(char* pData, ClientSession* pSocket);
	void Login(char* pData, ClientSession* pSocket);

private:
};