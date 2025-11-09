#include "pch.h"
#include "Monster.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

extern RECT gBackgoundRect;
std::uniform_int_distribution<> randWidth(gBackgoundRect.left, gBackgoundRect.right);
std::uniform_int_distribution<> randHeight(gBackgoundRect.top, gBackgoundRect.bottom);

// 임시지정
#define MONSTER_SPEED 2

Monster::Monster()
    : GameObject()
{
    _status._hp = 10;
    _status._speed = MONSTER_SPEED;
    _pos = { randWidth(gen), randHeight(gen) };

    _stateMachine->Start();
}

Monster::Monster(State* state)
    : GameObject(state)
{
    _status._hp = 10;
    _status._speed = MONSTER_SPEED;
    _pos = { randWidth(gen), randHeight(gen) };

    _stateMachine->Start();
}

void Monster::FindTarget(GameObject* other)
{
    SetTargetPos({ randWidth(gen), randHeight(gen) });
}

void Monster::Move()
{
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
    }
    else if (direct.x > 0) {
        _curFrame.y = 1;
    }
    if (direct.y <= 0) {
        _curFrame.y = 0;
    }
    else if (direct.y > 0) {
        _curFrame.y = 2;
    }
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Monster::Update(GameObject* other)
{
    _stateMachine->Update(other);
}

void Monster::Damaged(int damage)
{
    _status._hp -= damage;

    if (_status._hp <= 0) {
        _isDead = true;
    }
}
