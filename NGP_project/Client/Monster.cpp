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
    : _stateMachine{ new StateMachine{this, new FindTargetState}}
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

void Monster::Move()
{
    _prevPos = _pos;
    int dx = _targetPos.x - _pos.x;
    int dy = _targetPos.y - _pos.y;
    Vertex direct{ dx, dy };
    double distance = sqrt(dx * dx + dy * dy);

    if (distance <= _status._speed) {
        // 목표 위치에 도달했을 경우
        _pos = _targetPos;
    }
    else {
        // 일정한 속도로 이동
        double ratio = _status._speed / distance;
        _pos.x += static_cast<int>(dx * ratio);
        _pos.y += static_cast<int>(dy * ratio);
    }

    if (direct.x <= 0) {
        _curFrame.y = 3;
        _dir = Dir::Left;
    }
    else if (direct.x > 0) {
        _curFrame.y = 1;
        _dir = Dir::Right;
    }
    if (direct.y <= 0) {
        _curFrame.y = 0;
        _dir = Dir::Up;
    }
    else if (direct.y > 0) {
        _curFrame.y = 2;
        _dir = Dir::Down;
    }
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
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

void Monster::SetDirAndFrame(Dir dir)
{
    switch (dir)
    {
    case Dir::Left:        _curFrame.y = 3; break;
    case Dir::Right:       _curFrame.y = 1; break;
    case Dir::Up:          _curFrame.y = 0; break;
    case Dir::Down:        _curFrame.y = 2; break;
    }

    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}
