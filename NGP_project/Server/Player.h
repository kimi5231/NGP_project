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
	//std::shared_ptr<Item> _item;
};

