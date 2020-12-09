#include "server_type.h"
#include "BaseServer.h"

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
}