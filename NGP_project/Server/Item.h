#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item();
	Item(Vertex pos);
	virtual ~Item() {};

public:
	virtual void ChangeState(GameObject* player);
	void Expired(GameObject* player);

	ItemType GetItemType() const { return _type; }
private:
	ItemType _type{};
};