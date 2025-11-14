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
	void Move(Vertex);
public:

	std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	float _itemTimer{};
};

