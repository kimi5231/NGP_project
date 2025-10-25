#pragma once

// GameObject 그대로 사용X
class GameObject
{
public:
	GameObject() {};
	GameObject(HINSTANCE hInst);
	virtual ~GameObject();

	virtual void Init(HINSTANCE hInst) = 0;
	virtual void Render(HDC hDC, HDC srcDC);
public:
	void SetState(ObjectState state) { _state = state; }

protected:
	HINSTANCE _hInst{};

	ObjectState _state{};

	HBITMAP _bitmap{};
	HBITMAP _bitmapMask{};
	Vertex _spriteCnt{};	// 스프라이트 시트의 가로/세로 칸 개수

	Vertex _pos{};
	Vertex _curFrame{};
};