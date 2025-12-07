#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item();
	Item(Vertex pos);
	virtual ~Item() {};

public:
	void Update() override;
	ItemType GetItemType() const { return _type; }
private:
	ItemType _type{};
};