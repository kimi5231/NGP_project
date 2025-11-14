#pragma once

class GameScene;

class GameNetwork
{
public:
	GameNetwork();
	~GameNetwork();

public:
	void Update();

private:
	template<class T>
	void ProcessSend(PacketID id, const T& packet);

	void ProcessRecv();

public:
	template <class T>
	std::vector<char> CreatePacket(PacketID id, T& packet);

public:
	void SetGameScene(GameScene* scene) { _gameScene = scene; }

private:
	SOCKET _socket;

	fd_set _readSet{};
	fd_set _writeSet{};

	GameScene* _gameScene{};
};
