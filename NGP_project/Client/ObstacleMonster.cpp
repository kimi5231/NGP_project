#include "pch.h"
#include "Monster.h"
#include "ObstacleMonster.h"
#include "Global.h"

ObstacleMonster::ObstacleMonster() : Monster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"ObstacleMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"ObstacleMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _spriteCnt = { 8, 5 };
    _isFollow = false;
}

void ObstacleMonster::UseSkill()
{

}
