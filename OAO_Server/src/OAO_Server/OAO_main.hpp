#pragma once
#include "boost/asio.hpp"
#include "exit_code.h"
#include <exception>
#include "BaseServer.h"

using namespace boost;

namespace OAO
{
	template <typename TBaseServer>
	auto main() -> exit_code_underlying
	{
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
			//else
			//{
			//	server.Shutdown();
			//}
		}
		catch (std::exception &e)
		{
			exit(exit_code::failure);
		}

		return static_cast<exit_code_underlying>(exit_code::success);;
	}
}