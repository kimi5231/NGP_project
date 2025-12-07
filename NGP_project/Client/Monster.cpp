#include "pch.h"
#include <random>
#include "Monster.h"
#include "Global.h"
#include "Constant.h"
#include "Item.h"

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> randWidth(gBackgroundRect.left + CELL_SIZE, gBackgroundRect.right - CELL_SIZE);
std::uniform_int_distribution<> randHeight(gBackgroundRect.top + CELL_SIZE, gBackgroundRect.bottom - CELL_SIZE);


Monster::Monster()
{

}

Monster::Monster(ObjectState state)
{
    SetState(state);
}


void Monster::SetDirAndFrame(Dir dir)
{
    switch (dir)
    {
    case Dir::Left:        _curFrame.y = 3; break;
    case Dir::Right:       _curFrame.y = 1; break;
    case Dir::Up:          _curFrame.y = 0; break;
    case Dir::Down:        _curFrame.y = 2; break;
    }

    SetDir(dir);
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}