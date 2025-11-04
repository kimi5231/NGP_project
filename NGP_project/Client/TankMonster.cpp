#include "pch.h"
#include "Monster.h"
#include "TankMonster.h"
#include "Global.h"

TankMonster::TankMonster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"TankMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"TankMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _spriteCnt = { 8, 4 };
}

void TankMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
