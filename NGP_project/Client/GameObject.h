#pragma once
#include "StateMachine.h"

class BoundingBox;

using SpawnCallback = std::function<void(GameObject*)>;

typedef struct Status {
	int _speed{};
	int _hp{};
}Status;

class GameObject
{
public:
	GameObject();
	GameObject(State* state);
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

	// stateMachine 필요 함수
	StateMachine* GetStateMachine() { return _stateMachine.get(); }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }
	virtual void FindTarget(GameObject* other) {};

	// 충돌 관련
	BoundingBox GetBoundingBox() const;
	BoundingBox GetTargetBoundingBox() const;	// 타겟에 도작했는지 확인용
	bool IsCollision(const GameObject* other) const;
	bool IsClick(const POINT mouse) const;
	bool IsArrive() const;

	void SetToDead() { _isDead = true; }
	bool IsDead() const { return _isDead; }
public:
	void ResetCurFrame();

protected:
	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	Vertex _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수

	ObjectType _type{};
	Dir _dir{};
	Vertex _pos{};
	Vertex _targetPos{};
	Vertex _curFrame{};

	std::unique_ptr<StateMachine> _stateMachine;

	Status _status{};
	bool _isDead{};
	Vertex _size{ CELL_SIZE, CELL_SIZE };
};