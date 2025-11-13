#include "pch.h"
#include <random>
#include "Monster.h"

std::random_device rd;
std::mt19937 gen(rd());

RECT gBackgroundRect{ 150, 50, FRAME_BUFFER_WIDTH - 150, FRAME_BUFFER_HEIGHT - 50 }; // 임시

std::uniform_int_distribution<> randWidth(gBackgroundRect.left, gBackgroundRect.right);
std::uniform_int_distribution<> randHeight(gBackgroundRect.top, gBackgroundRect.bottom);
#define MONSTER_SPEED 3 // 임시

Monster::Monster()
    : _stateMachine{ new StateMachine{this, new FindTargetState}}
{
    _status._hp = 10;
    _status._speed = MONSTER_SPEED;
    _pos = { randWidth(gen), randHeight(gen) };

    _stateMachine->Start();
}

Monster::Monster(ObjectState state)
    : _stateMachine{ new StateMachine{this, state} }, GameObject(state)
{
    _status._hp = 10;
    _status._speed = MONSTER_SPEED;
    _pos = { randWidth(gen), randHeight(gen) };
    SetState(state);

    _stateMachine->Start();
}

void Monster::FindTarget(GameObject* other)
{
    SetTargetPos({ randWidth(gen), randHeight(gen) });
}

bool Monster::IsArrive() const
{
    if (_pos.x - _targetPos.x <= 0.1 && _pos.y - _targetPos.y <= 0.1) {
        return true;
    }
    return false;
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
}

void Monster::Update(GameObject* other)
{
    _stateMachine->Update(other);
}

void Monster::Damaged(int damage)
{
    _status._hp -= damage;

    if (_status._hp <= 0) {
        SetState(ObjectState::Dead);
    }
}