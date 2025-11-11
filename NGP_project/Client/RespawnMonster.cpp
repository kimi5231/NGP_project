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
}

void RespawnMonster::Update(GameObject* other)
{
    Monster::Update(other);
    if (IsState(ObjectState::Revive)) {
        if (CheckTimer(_timer, RESPAWN_TIME)) {
            SetState(ObjectState::Alive);

            _stateMachine->ChangeState(new SetTargetState);
            _stateMachine->Start();
        }
    }
}

bool RespawnMonster::UseSkill()
{
    if (IsState(ObjectState::Revive)) {
        _curFrame.x = 0;
        _curFrame.y = 5;
        _status._hp = 10;   // 회복

        _stateMachine->ChangeState(new IdleState);
        _stateMachine->Start();

        return true;
    }

    return false;
}

void RespawnMonster::Damaged(int damage)
{
    _status._hp -= damage;

    if (_status._hp <= 0) {
        --_status._life;
        if(_status._life > 0)
            SetState(ObjectState::Revive);
        else
            SetState(ObjectState::Dead);
    }
}

void RespawnMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
