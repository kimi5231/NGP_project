#pragma once
#include "GameObject.h"
//#include "Item"

class Player : public GameObject
{
public:
	Player();
public:
	void Update();

private:
	//std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	float _itemTimer{};
};

