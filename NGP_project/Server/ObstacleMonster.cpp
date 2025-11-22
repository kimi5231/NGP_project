#include "pch.h"
#include "Monster.h"
#include "ObstacleMonster.h"
#include "StateMachine.h"

ObstacleMonster::ObstacleMonster() : Monster(ObjectState::Move)
{   
    _isFollow = false;
    _type = ObjectType::ObstacleMonster;
    Monster::FindTarget(nullptr);

    _stateMachine->Start();
    _canUseSkill = true;
}

bool ObstacleMonster::UseSkill()
{
    _stateMachine->ChangeState(new IdleState);
    _stateMachine->Start();

    return true;
}
