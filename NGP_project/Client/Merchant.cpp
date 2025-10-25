#include "pch.h"
#include "Merchant.h"
#include "Global.h"

Merchant::Merchant() : GameObject()
{
	Init();
}

void Merchant::Init()
{
	std::filesystem::path path = g_resourcePath / "Merchant.bmp";
	_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Merchant.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Merchant_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	_spriteCnt = { 1, 4 };
	_pos = { 230, 30 };
	_state = ObjectState::Idle;
}

void Merchant::Update()
{

}

void Merchant::Render(HDC hDC, HDC srcDC)
{
	if (_state != ObjectState::Idle)
	{
		Parent::Render(hDC, srcDC);
	}
}