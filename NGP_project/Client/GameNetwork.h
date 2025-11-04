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

private:
	SOCKET _socket;
};

