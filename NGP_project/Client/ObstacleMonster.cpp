#include "pch.h"
#include "Monster.h"
#include "ObstacleMonster.h"
#include "Global.h"
#include "StateMachine.h"

ObstacleMonster::ObstacleMonster() : Monster(new MoveToTargetState)
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"ObstacleMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"ObstacleMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _spriteCnt = { 8, 5 };
    _isFollow = false;
    Monster::FindTarget(nullptr);

    _stateMachine->Start();
}

bool ObstacleMonster::UseSkill()
{
    _stateMachine->ChangeState(new IdleState);
    _stateMachine->Start();
    _curFrame.x = 0;
    _curFrame.y = 4;

    return true;
}
