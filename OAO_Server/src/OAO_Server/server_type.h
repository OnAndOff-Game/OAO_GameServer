#pragma once
#include <cstdint>

namespace OAO
{
	using server_type_underlying = std::int16_t;
	enum class server_type : server_type_underlying {
		none = 0,
		login,
		world,
	};
}