#include "pch.h"
#include "Item.h"
#include "Global.h"
#include "Constant.h"

extern float bulletSpeed;
extern bool useLightning, useWaterWheel, useShotgun, useHourglass;

Item::Item(ItemType type)
{
	GameObject::_type = ObjectType::Item;
	_type = type;

	switch (type) {
	case ItemType::Life:
		// Life
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Life.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Life_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Magazine:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Magazine.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Magazine_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Lightning:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Lightning.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Lightning_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Waterwheel:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Waterwheel.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Waterwheel_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Coffee:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Coffee.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Coffee_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Shotgun:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Shotgun.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Shotgun_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case ItemType::Hourglass:
		_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Hourglass.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Hourglass_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	}

	_spriteCnt = { 0,0 };
}

Item::Item(ItemType type, Vertex pos)
	: Item(type)
{
	_pos = pos;
}

void Item::ChangeState(GameObject* player)
{
	switch (_type) {
	case ItemType::Life:
		player->_status._life++;
		break;
	case ItemType::Magazine:
		bulletSpeed = BULLET_TIME / ADD_SPEED;
		break;
	case ItemType::Lightning:
		useLightning = true;
		break;
	case ItemType::Waterwheel:
		useWaterWheel = true;
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED + ADD_SPEED;
		break;
	case ItemType::Shotgun:
		useShotgun = true;
		break;
	case ItemType::Hourglass:
		useHourglass = true;
		break;
	}
}

void Item::Expired(GameObject* player)
{
	switch (_type) {
	case ItemType::Magazine:
		bulletSpeed = BULLET_TIME;
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED;
		break;
	case ItemType::Waterwheel:
		useWaterWheel = false;
		break;
	case ItemType::Shotgun:
		useShotgun = false;
		break;
	case ItemType::Hourglass:
		useHourglass = false;
		break;
	}
}

void Item::Render(HDC hdc, HDC srcDC)
{
	GameObject::Render(hdc, srcDC);
}