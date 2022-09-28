#pragma once
<<<<<<< HEAD
#include "boost/asio.hpp"
#include "exit_code.h"
#include <exception>
#include "BaseServer.h"

using namespace boost;
=======
#include "BaseServer.h"
#include "exit_code.h"
#include <exception>
#include <string>
#include <iostream>
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90

namespace OAO
{
	template <typename TBaseServer>
	auto main() -> exit_code_underlying
	{
<<<<<<< HEAD
		asio::io_service service;
		asio::signal_set signals(service, SIGINT);

		try
		{
			BaseServer& server = TBaseServer::get_instance();
			signals.async_wait([&server](const boost::system::error_code& errorCode, int handler_id) {
				server.Shutdown();
			});

			if (server.Initialize() == result::success) {
				service.run();
			}
			else
			{
				server.Shutdown();
			}
		}
		catch (std::exception &e)
=======
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
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
		{
			exit(exit_code::failure);
		}

<<<<<<< HEAD
=======
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

>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
		return static_cast<exit_code_underlying>(exit_code::success);;
	}
}