<<<<<<< HEAD
#include "OAO_Server/server_type.h"
#include "Game_Server/GameServer.h"
auto OAO::GameServer::Shutdown() -> void
{
	BaseServer::Shutdown();
}

auto OAO::GameServer::Listen() -> void
{
}

auto OAO::GameServer::LoadData() -> OAO::result
{
	return OAO::result::success;
}

OAO::GameServer::GameServer() :
	BaseServer(server_type::world)
{

}
=======
#include "GameServer.h"
#include <sstream>
#include "OAO_Server/ConnectionManager.h"

using namespace OAO;

void GameServer::Init()
{
}

void GameServer::OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData)
{

	ClientSession* client = ConnectionManager::get_instance().GetClientSession(clientIndex);

	//if (mFunctionProcess[pData[2]].funcProcessPacket != nullptr)
	//{
	//	(this->*mFunctionProcess[pData[2]].funcProcessPacket)(pData, client);
	//}
}

void GameServer::Chat(char* pData, ClientSession* pSocket)
{
	PACKET_CHAT* packet{ reinterpret_cast<PACKET_CHAT*>(pData) };

	printf("[%d] Chat : %s\n", pSocket->GetIndex(), packet->msg);
}

void GameServer::Login(char* pData, ClientSession* pSocket)
{
	PACKET_LOGIN* packet{ reinterpret_cast<PACKET_LOGIN*>(pData) };

	printf("[%d] Login : %s\n", pSocket->GetIndex(), packet->nickName);
}
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
