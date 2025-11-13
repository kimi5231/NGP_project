#pragma once
#include "StateMachine.h"

class BoundingBox;

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
	GameObject(ObjectState);
	virtual ~GameObject();

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

public:
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
	virtual void Move() {}

private:
	virtual void Idle() {};
	//virtual void Move() {};
	virtual void Boom() {};
	virtual void Dead() {};

public:
	void SetBitmap(HBITMAP bitmap) { _bitmap = bitmap; }
	void SetSpriteVertex(Vertex Vertex) { _spriteCnt = Vertex; }

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }
	void SetPos(Vertex pos) { _pos = pos; }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }

	// 충돌 관련
	BoundingBox GetBoundingBox() const;
	BoundingBox GetTargetBoundingBox() const;	// 타겟에 도작했는지 확인용
	bool IsCollision(const GameObject* other) const;
	bool IsClick(const POINT mouse) const;
	bool IsArrive() const;

	void SetState(ObjectState state) { _state = state; }
	bool IsState(ObjectState state) const { if (_state == state) return true; return false; }
	bool CanDamage() const { return !_invincible; }
public:
	void ResetCurFrame();

	float _timer{};	// 총알, 폭탄 등에 사용
	Status _status{0, 10};
protected:
	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	Vertex _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수
	Vertex _curFrame{};

	ObjectType _type{};
	ObjectState _state;
	Dir _dir{};
	Vertex _pos{};
	Vertex _targetPos{};

	bool _invincible{};	// 무적 판정

	Vertex _size{ CELL_SIZE, CELL_SIZE };
};