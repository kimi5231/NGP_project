#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item(ItemType type);
	Item(ItemType type, Vertex pos);
	virtual ~Item() {};

public:
	virtual void ChangeState(GameObject* player);
	void Expired(GameObject* player);

	virtual void Render(HDC hdc, HDC srcDC);

	ItemType GetItemType() const { return _type; }
private:
	ItemType _type{};
};