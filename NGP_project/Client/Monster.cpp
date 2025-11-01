#include "pch.h"
#include "Monster.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> randWidth(0, FRAME_BUFFER_WIDTH);
std::uniform_int_distribution<> randHeight(0, FRAME_BUFFER_HEIGHT);

// 임시지정
#define MONSTER_SPEED 2

Monster::Monster()
{
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

    if (distance <= MONSTER_SPEED) {
        // 목표 위치에 도달했을 경우
        _pos = _targetPos;
    }
    else {
        // 일정한 속도로 이동
        double ratio = MONSTER_SPEED / distance;
        _pos.x += static_cast<int>(dx * ratio);
        _pos.y += static_cast<int>(dy * ratio);
    }

    if (direct.x <= 0) {
        _curFrame.y = 3;    // 시간 되면 대각선 백터로 구현
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

