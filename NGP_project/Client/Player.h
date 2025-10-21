#pragma once
class Player
{
public:
	Player(HINSTANCE hInst);

	void Render() {};
	void Render(HDC hDC);
private:
	HBITMAP _bitmap;
	HBITMAP _bitmapMask;
};

