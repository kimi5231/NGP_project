﻿#include "pch.h"
#include "Monster.h"
#include "RespawnMonster.h"
#include "Global.h"

RespawnMonster::RespawnMonster()
{
    if (!_bitmap || !_bitmapMask) 
    {
        _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "RespawnMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "RespawnMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    if (!_spriteCnt.x)
        _spriteCnt = { 8, 6 };
}