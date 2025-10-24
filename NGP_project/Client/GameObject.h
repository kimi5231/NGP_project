#pragma once

// GameObject 그대로 사용X
class GameObject
{
public:
	GameObject(HINSTANCE hInst);
	virtual ~GameObject();

	virtual void init(HINSTANCE hInst) {};
	virtual void Render(HDC hDC, HDC srcDC);

	static HBITMAP _bitmap;
	static HBITMAP _bitmapMask;
protected:
	Vertex _pos{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
};
