#include "OAO_Server/OAO_main.hpp"
#include "Game_Server/GameServer.h"

auto main() -> OAO::exit_code_underlying
{
	return OAO::main<OAO::GameServer>();
}