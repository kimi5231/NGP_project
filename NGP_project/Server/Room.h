#pragma once
class Room
{
public:
	Room();
	~Room();

public:
	void Update();

public:
	void EnterRoom();
	void LeaveRoom();

private:
	void AddObject();
	void RemoveObject();

public:
	//void SetID();
	//int GetID();
	//void SetState();
	//RoomState GetState();
};