#include "pch.h"
#include "GameObject.h"
#include "Monster.h"

// 임시지정
#define MONSTER_SPEED 0.1

Monster::Monster(HINSTANCE hInst) : GameObject(hInst)
{
	Init(hInst);
}

void Monster::Init(HINSTANCE hInst)
{
}

void Monster::Left()
{
    _pos.x -= MONSTER_SPEED;
    _curFrame.y = 3;    // 시간 되면 대각선 백터로 구현
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Monster::Right()
{
    _pos.x += MONSTER_SPEED;
    _curFrame.y = 1;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Monster::Up()
{
    _pos.y -= MONSTER_SPEED;
    _curFrame.y = 0;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;

}

void Monster::Down()
{
    _pos.y += MONSTER_SPEED;
    _curFrame.y = 2;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;

}