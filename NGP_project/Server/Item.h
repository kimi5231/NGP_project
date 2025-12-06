#pragma once
#include "GameObject.h"

class Item : public GameObject, public std::enable_shared_from_this<Item>
{
public:
	Item();
	Item(Vertex pos);
	virtual ~Item() {};

public:
	void Update() override;
	virtual void ChangeState(GameObject* player);
	void Expired(GameObject* player);
	ItemType GetItemType() const { return _type; }
private:
	ItemType _type{};
};