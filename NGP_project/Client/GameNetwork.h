#pragma once

class GameNetwork
{
public:
	GameNetwork();
	~GameNetwork();

public:
	void Update();

private:
	void ProcessSend();
	void processRecv();

public:
	template <class T>
	std::vector<char> CreatePacket(C_PacketID id, T& packet);

private:
	SOCKET _socket;
};
