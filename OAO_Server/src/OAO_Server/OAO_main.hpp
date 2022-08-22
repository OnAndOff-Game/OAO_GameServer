#pragma once
#include "BaseServer.h"
#include "exit_code.h"
#include <exception>
#include <string>
#include <iostream>

namespace OAO
{
	template <typename TBaseServer>
	auto main() -> exit_code_underlying
	{
		try
		{
			TBaseServer& server = TBaseServer::get_instance();

			if (server.Initialize() == result::success) {
				server.Init();
			}
			if (server.Listen() == result::success) {

			}
			if (server.Start() == result::success) {

			}

		}
		catch (std::exception & e)
		{
			exit(exit_code::failure);
		}

		printf("아무 키나 누를 때까지 대기합니다\n");
		while (true)
		{
			std::string inputCmd;
			std::getline(std::cin, inputCmd);

			if (inputCmd == "quit")
			{
				break;
			}
		}

		return static_cast<exit_code_underlying>(exit_code::success);;
	}
}