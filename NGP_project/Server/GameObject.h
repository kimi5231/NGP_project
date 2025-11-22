#pragma once
#include "StateMachine.h"

using SpawnCallback = std::function<void(GameObject*)>;

typedef struct Status {
	int _speed{};
	int _hp{};
	int _life{ 1 };
}Status;

class GameObject
{
public:
	GameObject();
	GameObject(ObjectState state);

	virtual bool Move() { return true; };
	virtual void Update() {};
	virtual void FindTarget(GameObject* other) {};

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }

	void SetID(int id) { _id = id; }
	int GetID() const { return _id; }
	void SetPos(Vertex pos);
	Vertex GetPos() { return _pos; }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }

	void SetState(ObjectState state) { _state = state; }
	bool IsState(ObjectState state) const { if (_state == state) return true; return false; }

	Status _status{};
protected:
	int _id{};
	ObjectType _type{};
	ObjectState _state{};

	Dir _dir{};
	Vertex _pos{};
	Vertex _prevPos{};
	Vertex _targetPos{};
	Vertex _size{ CELL_SIZE, CELL_SIZE };

	bool _invincible{};	// 무적 판정
};

