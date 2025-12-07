#pragma once
#include "GameObject.h"

class Monster : public GameObject
{
public:
	Monster();
	Monster(ObjectState state);
	virtual ~Monster() {};
public:
	void Init();
	// stateMachine 필요 함수
	StateMachine* GetStateMachine() { return _stateMachine.get(); }
	virtual void FindTarget(GameObject* other);	// 랜덤 타겟 지정
	bool GetIsFollow() const { return _isFollow; }
	bool CanUseSkill() const { return _canUseSkill; }
	bool Move();

	virtual void Update();
	// 외부에서 state을 위해 bool값 리턴 - true: state 변경
	virtual bool UseSkill() { return false; }
	void DropItem();
	void Damaged(int damage) override;
	void PushOther(MonsterRef other);

	void SetCallback(SpawnCallback cb) { _spawnCallback = cb; }
protected:
	SpawnCallback _spawnCallback;
	bool _isFollow{ true };
	bool _canUseSkill{ false };
	bool _isCollision{ false };

	std::unique_ptr<StateMachine> _stateMachine;
};