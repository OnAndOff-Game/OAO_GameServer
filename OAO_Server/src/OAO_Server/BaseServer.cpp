#include "BaseServer.h"


namespace OAO
{
	BaseServer::BaseServer()
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