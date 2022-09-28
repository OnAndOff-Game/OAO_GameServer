#pragma once
<<<<<<< HEAD
#include "OAO_Server/types.h"
#include "OAO_Server/BaseServer.h"

namespace OAO
{
	class GameServer final : public BaseServer
	{ 
		SINGLETON(GameServer);
	//public: 
	//	static auto get_instance() -> GameServer& {
	//		static GameServer s_singleton; 
	//		return s_singleton;
	//}
	public:
		auto Shutdown() -> void override;

	protected:
		auto Listen() -> void;
		auto LoadData() -> result override;
	private:

	};
}
=======
#include "OAO_Server/BaseServer.h"
using namespace OAO;

class GameServer final : public BaseServer
{
public:
	static auto get_instance() -> GameServer& {
		static GameServer s_singleton;
		return s_singleton;
	}
	virtual void Init() override;
	virtual void OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData) override;
	void Chat(char* pData, ClientSession* pSocket);
	void Login(char* pData, ClientSession* pSocket);

private:
};
>>>>>>> edc06df0c588147a335279343a8d0f92cb03fb90
