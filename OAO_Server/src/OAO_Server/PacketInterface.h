#pragma once

#include <stdexcept>

namespace OAO {
	class PacketBuilder;
	class PacketReader;

	template <typename T>
	struct PacketSerialize {
		auto Read(PacketReader& reader) -> T {
			static_assert(false, "�������� ���� �����Դϴ�.");
			throw std::logic_error{ "�������� ���� �����Դϴ�." };
		}

		auto Write(PacketBuilder& builder, const T& obj) -> void {
			static_assert(false, "�������� ���� �����Դϴ�.");
			throw std::logic_error("�������� ���� �����Դϴ�.");
		}
	};
}