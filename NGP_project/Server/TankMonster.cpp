#include "pch.h"
#include "Monster.h"
#include "TankMonster.h"

TankMonster::TankMonster()
{
    _type = ObjectType::TankMonster;
    _status._hp = 20;
}

void TankMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
