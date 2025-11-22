#include "pch.h"
#include "Monster.h"
#include "NormalMonster.h"

NormalMonster::NormalMonster()
{
    _type = ObjectType::NormalMonster;
}

void NormalMonster::FindTarget(GameObject* other)
{
    SetTargetPos(other->GetPos());
}
