#pragma once

#include "PacketInterface.h"
#include "types.h"

namespace OAO
{
	class PacketReader
	{
	public:
		PacketReader() = default;
		PacketReader(unsigned char* buffer, size_t length);

		template<typename TValue>
		auto get()->TValue;
		template<typename TValue>
		auto unk()->TValue;
		template<typename TValue>
		auto get(size_t size)->TValue;
		template<typename TValue>
		auto peek()->TValue;
		template<typename TValue>
		auto skip()->PacketReader&;
	};
}