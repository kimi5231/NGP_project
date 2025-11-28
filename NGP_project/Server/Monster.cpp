#include "pch.h"
#include <random>
#include "Monster.h"
#include "Item.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"

RECT gBackgroundRect{ 150, 50, FRAME_BUFFER_WIDTH - 170, FRAME_BUFFER_HEIGHT - 70 };	// 변경 시 클라도 동일하게 변경 필요

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> randWidth(gBackgroundRect.left + CELL_SIZE, gBackgroundRect.right - CELL_SIZE);
std::uniform_int_distribution<> randHeight(gBackgroundRect.top + CELL_SIZE, gBackgroundRect.bottom - CELL_SIZE);


Monster::Monster()
    : _stateMachine{ new StateMachine{this, new FindTargetState} }
{
    Init();

}

Monster::Monster(ObjectState state)
    : _stateMachine{ new StateMachine{this, state} }, GameObject(state)
{
    SetState(state);
    Init();
}

void Monster::Init()
{
    _status._hp = 10;
    _status._speed = MONSTER_SPEED;

    std::uniform_int_distribution<> randSpawn(0, 3);
    int cellOffset = 8 * CELL_SIZE;
    switch (randSpawn(gen)) {
    case 0:
        // 북쪽
        _pos = { float(gBackgroundRect.left + cellOffset),
                 float(gBackgroundRect.top + CELL_SIZE / 2) };
        break;
    case 1:
        // 동쪽
        _pos = { float(gBackgroundRect.right - CELL_SIZE / 2),
                 float(gBackgroundRect.top + cellOffset) };
        break;
    case 2:
        // 남쪽
        _pos = { float(gBackgroundRect.left + cellOffset),
                 float(gBackgroundRect.bottom - CELL_SIZE / 2) };
        break;
    case 3:
        // 서쪽
        _pos = { float(gBackgroundRect.left + CELL_SIZE / 2),
                 float(gBackgroundRect.top + cellOffset) };
        break;
    default:
        break;
    }
    _prevPos = _pos;
    _stateMachine->Start();
}

void Monster::FindTarget(GameObject* other)
{
    SetTargetPos({ float(randWidth(gen)),
                   float(randHeight(gen)) });
}

bool Monster::Move()
{
    _prevPos = _pos;
    float dx = _targetPos.x - _pos.x;
    float dy = _targetPos.y - _pos.y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance <= _status._speed) {
        // 목표 위치에 도달했을 경우
        _pos = _targetPos;
        return true;
    }

    // 일정한 속도로 이동
    int dirX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
    int dirY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
    _pos.x += dirX * _status._speed;
    _pos.y += dirY * _status._speed;
    
    // 방향 설정
    if (dx < 0) _dir = Dir::Left;
    else if (dx > 0) _dir = Dir::Right;
    if (dy < 0) _dir = Dir::Up;
    else if (dy > 0) _dir = Dir::Down;

    g_framework->SendMovePacket(shared_from_this());

    return true;
}

void Monster::Update(GameObject* other)
{
    _stateMachine->Update(other);
}

void Monster::DropItem()
{
    std::uniform_int_distribution<> randSpawn(static_cast<int>(ItemType::Life), static_cast<int>(ItemType::Hourglass));

    GameObject* item = new Item(static_cast<ItemType>(randSpawn(gen)), _pos);
    _spawnCallback(item);
}

void Monster::Damaged(int damage)
{
    _status._hp -= damage;

    if (_status._hp <= 0) {
        _stateMachine->ChangeState(new DeadState);
        _stateMachine->Start();
    }
}