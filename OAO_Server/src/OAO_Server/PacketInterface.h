#pragma once

#include <stdexcept>

namespace OAO {
	class PacketBuilder;
	class PacketReader;

	template <typename T>
	struct PacketSerialize {
		auto Read(PacketReader& reader) -> T {
			static_assert(false, "적절하지 않은 유형입니다.");
			throw std::logic_error{ "적절하지 않은 유형입니다." };
		}

		auto Write(PacketBuilder& builder, const T& obj) -> void {
			static_assert(false, "적절하지 않은 유형입니다.");
			throw std::logic_error("적절하지 않은 유형입니다.");
		}
	};
}