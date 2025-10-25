#include "pch.h"
#include "Item.h"
#include "Global.h"

Item::Item(ItemType type)
{
	_type = type;

	switch (type)
	{
	case ItemType::Life:
		// Life
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Life.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Life_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Magazine:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Magazine.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Magazine_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Lightning:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Lightning.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Lightning_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Waterwheel:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Waterwheel.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Waterwheel_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Coffee:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Coffee.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Coffee_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Shotgun:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Shotgun.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Shotgun_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Hourglass:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Hourglass.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Hourglass_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	}

	_spriteCnt = { 0,0 };
}

void Item::Update()
{
}

void Item::Render(HDC hdc, HDC srcDC)
{
	GameObject::Render(hdc, srcDC);
}