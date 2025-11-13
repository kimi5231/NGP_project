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

    if (_canUseSkill) {
        if (CheckTimer(_timer, RESPAWN_TIME)) {
            SetState(ObjectState::Move);

            _stateMachine->ChangeState(new FindTargetState);
            _stateMachine->Start();
            _canUseSkill = false;
            _invincible = false;
        }
    }
}

bool RespawnMonster::UseSkill()
{
    _curFrame.x = 0;
    _curFrame.y = 5;
    _status._hp = 10;   // 회복

    _stateMachine->ChangeState(new IdleState);
    _stateMachine->Start();

    return true;
}

void RespawnMonster::Damaged(int damage)
{
    _status._hp -= damage;

    if (_status._hp <= 0) {
        --_status._life;
        if (_status._life > 0) {
            _canUseSkill = true;
            _invincible = true;
        }
        else
            SetState(ObjectState::Dead);
    }
}

void RespawnMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
