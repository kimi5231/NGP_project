#include "pch.h"
#include "Monster.h"
#include "TankMonster.h"

TankMonster::TankMonster()
{
    _type = ObjectType::TankMonster;

}

void TankMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
