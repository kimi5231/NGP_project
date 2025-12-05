#pragma once
class BoundingBox;

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
	GameObject(ObjectType, Vertex);
	virtual ~GameObject();

public:
	virtual void Update();
	virtual void Render(HDC hdc, HDC srcDC);
public:
	virtual void Move() {}
public:
	void SetBitmap(HBITMAP bitmap) { _bitmap = bitmap; }
	void SetSpriteVertex(POINT Vertex) { _spriteCnt = Vertex; }

	void SetId(int id) { _id = id; }
	int GetId() { return _id; }
	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }

	void SetPos(Vertex pos) { _pos = pos; }
	Vertex GetTargetPos() { return _targetPos; }
	void SetTargetPos(Vertex target) { _targetPos = target; }
	Vertex GetPos() { return _pos; }
	Vertex GetPrevPos() { return _prevPos; }

	Dir GetDir() const { return _dir; }
	void SetDir(Dir dir) { _dir = dir; }

	void SetTimer(const int timer) { _timer = timer; }

	// 충돌 관련
	BoundingBox GetBoundingBox() const;
	bool IsCollision(const GameObject* other) const;
	bool IsClick(const POINT mouse) const;

	void SetState(ObjectState state) { _state = state; }
	ObjectState GetState() { return _state; }
	bool IsState(ObjectState state) const { if (_state == state) return true; return false; }
public:
	void ResetCurFrame();

	int _timer{};	// 총알, 폭탄 등에 사용
	Status _status{0, 10};
protected:
	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	POINT _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수
	POINT _curFrame{};

	int _id{};
	ObjectType _type{};
	ObjectState _state;
	Dir _dir{};
	Vertex _pos{};
	Vertex _prevPos{};
	Vertex _targetPos{};

	bool _invincible{};	// 무적 판정

	Vertex _size{ CELL_SIZE, CELL_SIZE };
};