#pragma once
#include "GameObject.h"

class Item;

class Player : public GameObject 
{
public:
	Player();
public:
	virtual void Update();
	void UseItem();
	void SetItem(ItemRef item);
	void Move(Vertex vecDir, Dir dir);

	void SetDirAndFrame(Dir dir);

	void ClampPlayerArea();

	Vertex GetPrevSendPos() const { return _prevSendPos; }
	void   SetPrevSendPos(const Vertex& pos) { _prevSendPos = pos; }

	Dir GetPrevDir() const { return _prevDir; }
	void SetPrevDir(Dir dir) { _prevDir = dir; }

public:
	std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	int _itemTimer{};

	Vertex _prevSendPos{};
	Dir _prevDir{};
};

