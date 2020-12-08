#pragma once
#include "OAO_Server/types.h"
#include "OAO_Server/BaseServer.h"

namespace OAO
{
	class GameServer final : public OAO::BaseServer
	{ 
		SINGLETON(GameServer);

	public:
		auto Shutdown() -> void override;

	protected:
		auto Listen() -> void;
		auto LoadData()->OAO::result override;
	private:
	};
}