#include "pch.h"
#include "Monster.h"
#include "BomberMonster.h"
#include "BombObject.h"

BomberMonster::BomberMonster()
{
    _isFollow = false;
    _canUseSkill = true;
    _type = ObjectType::BomberMonster;
}
bool BomberMonster::UseSkill()
{
    GameObject* bomb = new BombObject(_pos);
    //_spawnCallback(bomb);

    return false;
}
