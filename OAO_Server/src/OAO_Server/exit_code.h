#pragma once
namespace OAO
{
	using exit_code_underlying = int;
	enum class exit_code : exit_code_underlying
	{
		success = 0,
		failure = 1,
	};

	auto exit(exit_code code) -> void;
}