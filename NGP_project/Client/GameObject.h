#pragma once

class GameObject
{
public:
	GameObject(HINSTANCE hInst);

	void Render() {};
	virtual void Render(HDC hDC);
	virtual void RenderMask(HDC hDC, HDC srcDC);	// hDC에 srcDC 있는 비트맵 그리는 함수
	virtual void RenderOriginal(HDC hDC, HDC srcDC);
private:
	HBITMAP _bitmap;		// static으로 수정하기
	HBITMAP _bitmapMask;
};

