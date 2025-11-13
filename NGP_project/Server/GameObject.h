#pragma once
#include "StateMachine.h"

using SpawnCallback = std::function<void(GameObject*)>;

typedef struct Status {
	int _speed{};
	int _hp{};
}Status;

class GameObject
{
public:
	GameObject();
	GameObject(ObjectState state);

	virtual void Move() {};
	virtual void Update() {};
	virtual void FindTarget(GameObject* other) {};

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }

	void SetPos(Vertex pos) { _pos = pos; }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }

	void SetState(ObjectState state) { _state = state; }
	bool IsState(ObjectState state) const { if (_state == state) return true; return false; }
protected:
	ObjectType _type{};
	ObjectState _state{};

	Dir _dir{};
	Vertex _pos{};
	Vertex _targetPos{};
	Vertex _size{ CELL_SIZE, CELL_SIZE };

	bool _invincible{};	// 무적 판정

	Status _status{};
};

