#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
public:
	void Update();
	void UseItem();
	void SetItem(ItemRef item);
	bool SetPos(Vertex pos) override;

	void SetCollision() { _isCollision = true; }
private:
	std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	float _itemTimer{};
	bool _isCollision{};	// 장애물과 충돌 여부 체크
};

