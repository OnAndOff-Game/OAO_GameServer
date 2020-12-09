#pragma once

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
	enum class result {
		success,
		failure,
	};

	using server_type_underlying = int16_t;
	enum class server_type : server_type_underlying {
		none = 0,
		login,
		world,
	};
}