#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	Monster(State* state);
	virtual ~Monster() {};
public:
	virtual void FindTarget(GameObject* other) override;	// 랜덤 타겟 지정
	bool GetIsFollow() const { return _isFollow; }
	virtual void Move() override;

	virtual void Update(GameObject* other);
	// 외부에서 state을 위해 bool값 리턴 - true: state 변경
	virtual bool UseSkill() { return false; }
	virtual void Damaged(int damage);

	void SetCallback(SpawnCallback cb) { _spawnCallback = cb; }
protected:
	SpawnCallback _spawnCallback;
	bool _isFollow{ true };
};