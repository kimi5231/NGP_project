#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	virtual ~Monster() {};
public:
	virtual void FindTarget(GameObject* other) override;	// 랜덤 타겟 지정
	bool GetIsFollow() const { return _isFollow; }
	virtual void Move() override;

	virtual void Update(GameObject* other);
	virtual void UseSkill() {};

	void SetCallback(SpawnCallback cb) { _spawnCallback = cb; }
protected:
	SpawnCallback _spawnCallback;
	bool _isFollow{ true };
};