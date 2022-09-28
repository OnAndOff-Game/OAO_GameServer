#pragma once
#include "types.h"
#include <vector>

namespace OAO {
	namespace Packets {
		namespace Handle {
			class PacketBuilder {
			public:
				PacketBuilder();

				template <typename TValue>
				PacketBuilder& Add(const TValue& value);
				template <typename TValue>
				PacketBuilder& Unknown();
				template <typename TValue>
				PacketBuilder& Defer();
				template <typename TValue>
				PacketBuilder& Unknown(const TValue& value);
				PacketBuilder& Unknown(int32_t bytes);

				template <typename TValue>
				PacketBuilder& Add(const TValue& value, size_t count);
			};
		}
	}
}