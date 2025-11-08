#include "pch.h"
#include "Monster.h"
#include "BomberMonster.h"
#include "BombObject.h"
#include "Global.h"

BomberMonster::BomberMonster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"BomberMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"BomberMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _spriteCnt = { 8, 4 };
    _isFollow = false;
}

bool BomberMonster::UseSkill()
{
    GameObject* bomb = new BombObject(_pos);
    _spawnCallback(bomb);

    return false;
}
