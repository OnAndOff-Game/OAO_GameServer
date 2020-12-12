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