#include "pch.h"
#include "Monster.h"
#include "RespawnMonster.h"
#include "Global.h"

RespawnMonster::RespawnMonster() : Monster()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"RespawnMonster.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"RespawnMonster_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 8, 6 };
}

bool RespawnMonster::UseSkill()
{
    if (IsDead()) {
        _curFrame.x = 0;
        _curFrame.y = 5;
        _status._hp = 10;   // 회복

        _stateMachine->ChangeState(new IdleState);
        _stateMachine->Start();

        return true;
    }

    return false;
}

void RespawnMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
