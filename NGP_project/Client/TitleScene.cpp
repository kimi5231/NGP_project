#include "pch.h"
#include "TitleScene.h"
#include "Global.h"
#include "GameObject.h"

TitleScene::TitleScene()
{
	// Create Title
	_backgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Title.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Render(HDC hdc)
{
	BITMAP bmpInfo;
	GetObject(_backgroundBitmap, sizeof(BITMAP), &bmpInfo);
	HDC memDC = CreateCompatibleDC(hdc);

	SelectObject(memDC, _backgroundBitmap);
	StretchBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
}