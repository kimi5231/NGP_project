#include "pch.h"
#include "Monster.h"
#include "RespawnMonster.h"
#include "Constant.h"

RespawnMonster::RespawnMonster() : Monster()
{
    _status._life = 2;
    _type = ObjectType::RespawnMonster;
}

void RespawnMonster::Update()
{
    Monster::Update();

    if (_canUseSkill) {
        // 시간 재는 함수 추가 시 주석 풀기
        /*if (CheckTimer(_timer, RESPAWN_TIME)) {
            SetState(ObjectState::Move);

            _stateMachine->ChangeState(new FindTargetState);
            _stateMachine->Start();
            _canUseSkill = false;
            _invincible = false;
        }*/
    }
}

bool RespawnMonster::UseSkill()
{
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
        else {
            _stateMachine->ChangeState(new DeadState);
            _stateMachine->Start();
        }
    }
}

void RespawnMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
