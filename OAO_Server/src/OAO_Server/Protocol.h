#pragma once

namespace OAO
{
	constexpr int LISTEN_PORT = 6000;

	constexpr int MAX_BUF_SIZE = 1024;
	constexpr int MAX_PACKET_SIZE = 255;
	constexpr int MAX_USER_COUNT = 500;

	constexpr UINT64 RE_USE_SESSION_WAIT_TIMESEC = 3;

	enum class result {
		success,
		failure,
	};

	enum class IOOp
	{
		ACCEPT,
		RECV,
		SEND
	};

	enum PacketType
	{
		LOGIN = 0,
		CHAT = 1,
	};

	struct stOverlappedEx
	{
		WSAOVERLAPPED mWsaOverlapped;
		WSABUF mWsaBuf;
		IOOp mOperation;
		UINT32 SessionIndex = 0;
	};
}