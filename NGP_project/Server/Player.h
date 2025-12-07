#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();

public:
	void Update();
	void UseItem();

public:
	ItemRef GetItem() { return _item.first; }
	void SetItem(ItemRef item);
	void Damaged(int damage) override;
private:
	std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	float _itemTimer{};
	float _invincibleTimer{};	// 무적 시간
};