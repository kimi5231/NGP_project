#include "pch.h"
#include "GameObject.h"
#include "Monster.h"
#include "TankMonster.h"

TankMonster::TankMonster(HINSTANCE hInst) : Monster(hInst)
{
	Init(hInst);
}

void TankMonster::Init(HINSTANCE hInst)
{
    if (!_bitmap || !_bitmapMask) {
        _bitmap = (HBITMAP)LoadImage(hInst, TEXT("tank.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("tank_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    if (!_spriteCnt.x) {
        _spriteCnt = { 8, 4 };
    }
}
