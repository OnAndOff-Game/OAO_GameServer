#pragma once
#include "OAO_Server/types.h"
#include "OAO_Server/BaseServer.h"

namespace OAO
{
	class GameServer final : public BaseServer
	{ 
		SINGLETON(GameServer);
	//public: 
	//	static auto get_instance() -> GameServer& {
	//		static GameServer s_singleton; 
	//		return s_singleton;
	//}
	public:
		auto Shutdown() -> void override;

	protected:
		auto Listen() -> void;
		auto LoadData() -> result override;
	private:

	};
}