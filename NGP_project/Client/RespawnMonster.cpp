#include "pch.h"
#include "GameObject.h"
#include "Monster.h"
#include "RespawnMonster.h"
#include "Global.h"

RespawnMonster::RespawnMonster() : Monster()
{
	Init();
}

void RespawnMonster::Init()
{
    if (!_bitmap || !_bitmapMask) {
        _bitmap = (HBITMAP)LoadImage(hInst, TEXT("respawn.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("respawn_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    if (!_spriteCnt.x) {
        _spriteCnt = { 8, 6 };
    }
}
