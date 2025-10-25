#include "pch.h"
#include "GameObject.h"
#include "Monster.h"
#include "BomberMonster.h"
#include "Global.h"

BomberMonster::BomberMonster() : Monster()
{
	Init();
}

void BomberMonster::Init()
{
    if (!_bitmap || !_bitmapMask) {
        _bitmap = (HBITMAP)LoadImage(hInst, TEXT("bomber.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("bomber_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    if (!_spriteCnt.x) {
        _spriteCnt = { 8, 4 };
    }
}
