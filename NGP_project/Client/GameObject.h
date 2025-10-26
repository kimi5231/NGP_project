#pragma once
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

private:
	virtual void Idle() {};
	virtual void Move() {};
	virtual void Boom() {};
	virtual void Dead() {};

public:
	void SetBitmap(HBITMAP bitmap) { _bitmap = bitmap; }
	void SetSpriteCount(Vertex count) { _spriteCnt = count; }

	void SetObjectType(ObjectType type) { _type = type; }
	ObjectType GetObjectType() { return _type; }
	void SetPos(Vertex pos) { _pos = pos; }

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
};