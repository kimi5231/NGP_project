#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item(ItemType type);
	virtual ~Item() {};

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

private:
	ItemType _type{};
};