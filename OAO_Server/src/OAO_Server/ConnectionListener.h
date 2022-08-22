#pragma once

#include <boost/asio.hpp>
#include "types.h"

namespace OAO
{
	class ConnectionManager;

	class ConnectionListener
	{
	public:
		ConnectionListener(
			boost::asio::io_service &ioService,
			boost::asio::ip::tcp::endpoint endPoint,
			ConnectionManager &manager);

	public:
		auto BeginAccept() -> void;
		auto Stop() -> void;

	private:
		boost::asio::ip::tcp::acceptor mAcceptor;
		ConnectionManager &mConnectionManager;
	};
}