#pragma once
#include "types.h"

namespace OAO
{
	class BaseServer
	{
	public:
		virtual ~BaseServer() = default;

		auto Initialize() -> result;
		virtual auto Shutdown() -> void;

	protected:
		BaseServer();
		virtual auto LoadData() -> result = 0;
	private:

	};
}