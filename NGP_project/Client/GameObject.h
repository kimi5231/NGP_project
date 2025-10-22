#pragma once

class GameObject
{
public:
	GameObject(HINSTANCE hInst);

	void Render() {};
	virtual void Render(HDC hDC);
private:
	HBITMAP _bitmap;		// static으로 수정하기
	HBITMAP _bitmapMask;
};

