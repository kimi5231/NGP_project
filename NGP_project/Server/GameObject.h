#pragma once
#include "StateMachine.h"

typedef struct Status {
	int _speed{};
	int _hp{};
}Status;

class GameObject
{
public:
	GameObject();
	GameObject(State* state);

	virtual void Move() {};
	virtual void FindTarget(GameObject* other) {};

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }
	void SetPos(Vertex pos) { _pos = pos; }

	// stateMachine 필요 함수
	StateMachine* GetStateMachine() { return _stateMachine.get(); }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }
protected:
	ObjectType _type{};
	Dir _dir{};
	Vertex _pos{};
	Vertex _targetPos{};
	Vertex _curFrame{};
	Vertex _spriteCnt{};
	Vertex _size{ CELL_SIZE, CELL_SIZE };

	std::unique_ptr<StateMachine> _stateMachine;

	Status _status{};
};

