#include "pch.h"
#include "Monster.h"
#include "NormalMonster.h"
#include "Global.h"

NormalMonster::NormalMonster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"NormalMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"NormalMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _spriteCnt = { 8, 4 };
}

void NormalMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
