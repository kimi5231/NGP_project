#pragma once
#include "StateMachine.h"

class BoundingBox;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);

public:
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
	virtual void Move() {};

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

	StateMachine* GetStateMachine() { return _stateMachine; }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }

	// 충돌 관련
	BoundingBox GetBoundingBox() const;
	BoundingBox GetTargetBoundingBox() const;	// 타겟에 도작했는지 확인용
	bool IsCollision(const GameObject* other) const;
	bool IsArrive() const;
public:
	void ResetCurFrame();

protected:
	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	Vertex _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수

	ObjectType _type{};
	Dir _dir{};
	Vertex _pos{};
	Vertex _curFrame{};

	int _speed{};

	StateMachine* _stateMachine;

	Vertex _targetPos{};
};