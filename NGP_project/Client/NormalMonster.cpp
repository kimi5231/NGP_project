#include "pch.h"
#include "GameObject.h"
#include "Monster.h"
#include "NormalMonster.h"
#include "Global.h"

NormalMonster::NormalMonster() : Monster()
{
	Init();
}

void NormalMonster::Init()
{
    if (!_bitmap || !_bitmapMask) {
        _bitmap = (HBITMAP)LoadImage(hInst, TEXT("normal_monster.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("normal_monster_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    if (!_spriteCnt.x) {
        _spriteCnt = { 8, 4 };
    }
}
