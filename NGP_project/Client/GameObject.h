#pragma once

class GameObject
{
public:
	GameObject(HINSTANCE hInst);

	void Render() {};
	virtual void Render(HDC hDC, HDC srcDC);
private:
	HBITMAP _bitmap;		// static으로 수정하기
	HBITMAP _bitmapMask;
};

