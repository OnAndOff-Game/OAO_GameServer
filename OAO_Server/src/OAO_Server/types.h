#pragma once
<<<<<<< HEAD
=======
#include <WinSock2.h>
#include <queue>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <functional>
#include <cstdint>
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90

#define NONCOPYABLE(TypeName) \
	public: \
		TypeName(const TypeName &) = delete; \
		auto operator=(const TypeName &) -> TypeName & = delete;

#define SINGLETON(TypeName) \
	public: \
		static auto get_instance() -> TypeName & { \
			static TypeName s_singleton; \
			return s_singleton; \
		} \
	NONCOPYABLE(TypeName); \
	private: \
		TypeName();

namespace OAO
{
<<<<<<< HEAD
	enum class result {
		success,
		failure,
	};
=======
#pragma pack(push, 1)
	struct PACKET_CHAT
	{
		uint16_t size{};
		char type{};
		char msg[255]{};
	};

	struct PACKET_LOGIN
	{
		uint16_t size{};
		char type{};
		char nickName[50]{};
	};
#pragma pack(pop)

	using int8_t = std::int8_t;
	using int16_t = std::int16_t;
	using int32_t = std::int32_t;
	using int64_t = std::int64_t;
	using uint8_t = std::uint8_t;
	using uint16_t = std::uint16_t;
	using uint32_t = std::uint32_t;
	using uint64_t = std::uint64_t;
	using string = std::string;

	template <typename TElement, size_t Size>
	using array = std::array<TElement, Size>;
	template <typename TElement>
	using queue = std::deque<TElement>;
	template <typename TKey, typename TElement, typename TOrdering = std::less<TKey>>
	using ordered_map = std::map<TKey, TElement, TOrdering>;
	template <typename TKey, typename TOrdering = std::less<TKey>>
	using ordered_set = std::set<TKey, TOrdering>;
	template <typename TKey, typename TElement>
	using hash_map = std::unordered_map<TKey, TElement>;
	template <typename TKey>
	using hash_set = std::unordered_set<TKey>;
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
}