#pragma once
#include "StateMachine.h"

using SpawnCallback = std::function<void(GameObject*)>;
class BoundingBox;

typedef struct Status {
	int _speed{};
	int _hp{};
	int _life{ 1 };
}Status;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	GameObject(ObjectState state);
	GameObject(ObjectType, Vertex);

	virtual bool Move() { return true; };
	virtual void Update() {};
	virtual void FindTarget(GameObject* other) {};
	bool IsArrive();

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType()const { return _type; }

	void SetID(int id) { _id = id; }
	int GetID() const { return _id; }
	virtual bool SetPos(Vertex pos);
	Vertex GetPos() const { return _pos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetTargetPos() const { return _targetPos; }
	void SetDir(Dir dir) { _dir = dir; }
	Dir GetDir() { return _dir; }

	void SetState(ObjectState state) { _state = state; }
	ObjectState GetState() { return _state; }
	bool IsState(ObjectState state) const { if (_state == state) return true; return false; }

	BoundingBox GetBoundingBox() const;
	bool IsCollision(const GameObjectRef other) const;

	virtual void Damaged(int damage) {};
	int GetDamage() const { return _damage; }
	bool CanDamage() const { return !_invincible; }
	void UndoPos();

	Status _status{};
	int _timer{};	// 총알, 폭탄 등에 사용
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
	int _damage{ 10 };
};

