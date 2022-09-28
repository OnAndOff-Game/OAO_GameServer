#pragma once

#include "PacketInterface.h"
#include "PacketReader.h"
#include <string>
#include <stdexcept>
namespace OAO
{
	class ip
	{
	public:
		class type {
		public:
			type(int8_t ipType) :
				mType(ipType)
			{
				if (ipType != ipv4 && ipType != ipv6) {
					throw std::invalid_argument("");
				}
			}

			auto operator==(const type& right) const -> bool { return right.mType == this->mType; }
			auto operator==(const int8_t& right) const -> bool { return right == this->mType; }
			auto operator!=(const type& right) const -> bool { return right.mType != this->mType; }
			auto operator!=(const int8_t& right) const -> bool { return right == this->mType; }

			static const int8_t ipv4 = 1;
			static const int8_t ipv6 = 2;
		private:
			friend class ip;
			friend struct PacketSerialize<ip::type>;
			type() = default;
			int8_t mType = -1;
		};

		ip(const std::string& adress, ip::type type);
		explicit ip(uint32_t ipv4);

		auto ToString() const -> std::string;
		auto AsIpv4()  const -> uint32_t;
		auto GetType() const -> const ip::type&;
		auto IsInitialized() const -> bool;

		auto operator==(const ip& right) const -> bool { return right.mType == this->mType && right.mIpv4 == this->mIpv4; }
		auto operator!=(const ip& right) const -> bool { return right.mType == this->mType || right.mIpv4 == this->mIpv4; }
		friend auto operator<<(std::ostream& out, const ip& ip)->std::ostream&;

		static auto StringToIpv4(const std::string& name) -> uint32_t;
	protected:
		friend class ExternalIp;
		friend struct PacketSerialize<ip>;
		ip() = default;

		uint32_t mIpv4 = 0;
		ip::type mType = ip::type::ipv4;
	};

	template <>
	struct PacketSerialize<ip>
	{
		auto Read(PacketReader& reader) -> ip::type {
			ip::type ret(reader.get<ip::type>());
			return ret;
		}
	};
}

