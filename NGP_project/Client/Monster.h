#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	virtual ~Monster() {};
public:
	virtual void FindTarget(GameObject* other) override;	// 랜덤 타겟 지정
	virtual void Move() override;

	virtual void Update(GameObject* other);
};