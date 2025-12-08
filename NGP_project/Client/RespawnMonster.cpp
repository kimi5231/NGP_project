#include "pch.h"
#include "Monster.h"
#include "RespawnMonster.h"
#include "Global.h"
#include "Constant.h"

RespawnMonster::RespawnMonster() : Monster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"RespawnMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"RespawnMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 8, 6 };
    _status._life = 2;
    _type = ObjectType::RespawnMonster;
}

void RespawnMonster::Update()
{
    if (_state == ObjectState::Idle) {
        _curFrame.x = 1;
        _curFrame.y = 5;
    }
}
