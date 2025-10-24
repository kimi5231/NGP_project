#pragma once

// GameObject 그대로 사용X
class GameObject
{
public:
	GameObject(HINSTANCE hInst);
	virtual ~GameObject();

	virtual void Init(HINSTANCE hInst) {};
	virtual void Render(HDC hDC, HDC srcDC);

protected:
	HBITMAP _bitmap;
	HBITMAP _bitmapMask;
	Vertex _spriteCnt;	// 스프라이트 시트의 가로/세로 칸 개수

	Vertex _pos{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
	Vertex _frame{};
};
