#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Render(HDC hDC, HDC srcDC);

public:
	void SetBitmap(HBITMAP bitmap) { _bitmap = bitmap; }
	void SetSpriteCount(Vertex count) { _spriteCnt = count; }

	void SetObjectType(ObjectType type) { _type = type; }
	void SetState(ObjectState state) { _state = state; }
	void SetPos(Vertex pos) { _pos = pos; }

public:
	void ResetCurFrame();

protected:
	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	Vertex _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수

	ObjectType _type{};
	ObjectState _state{};

	Vertex _pos{};
	Vertex _curFrame{};
};