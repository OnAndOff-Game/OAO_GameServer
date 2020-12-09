#pragma once
#include "types.h"

class server_type;
namespace OAO
{
	class BaseServer
	{
	public:
		virtual ~BaseServer() = default;

		auto Initialize() -> result;
		virtual auto Shutdown() -> void;

	protected:
		BaseServer(server_type serverType);
		virtual auto LoadData() -> result = 0;
	private:
		server_type mServerType;
	};
}