#include "pch.h"
#include "Monster.h"
#include "Item.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"
#include "TimeManager.h"

RECT gBackgroundRect{ 150, 50, FRAME_BUFFER_WIDTH - 170, FRAME_BUFFER_HEIGHT - 70 };	// 변경 시 클라도 동일하게 변경 필요

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
    if (_isCollision) return false;

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

    //float delta = GET_SINGLE(TimeManager)->GetDeltaTime();
    _pos.x += dirX * _status._speed;
    _pos.y += dirY * _status._speed;
    
    // 방향 설정
    if (dx < 0) _dir = Dir::Left;
    else if (dx > 0) _dir = Dir::Right;
    if (dy < 0) _dir = Dir::Up;
    else if (dy > 0) _dir = Dir::Down;

    if (abs(static_cast<int>(_prevPos.x) - static_cast<int>(_pos.x)) > 1 || abs(static_cast<int>(_prevPos.y) - static_cast<int>(_pos.y)) > 1)
    {
        _prevPos = _pos;
        g_framework->SendMovePacket(shared_from_this(), true);
    }

    return true;
}

void Monster::Update()
{
    _stateMachine->Update();
}

void Monster::DropItem()
{
    GameObject* item = new Item(_pos);
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

void Monster::PushOther(MonsterRef other)
{
    _isCollision = true;
    Vertex otherPos = other->GetPos();
    float dx = _pos.x - otherPos.x;
    float dy = _pos.y - otherPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist == 0) {
        // 완전히 같은 위치일 경우 임의로 밀어냄
        dx = 1.0f;
        dy = 0.0f;
        dist = 1.0f;
    }

    float overlap = (CELL_SIZE * 2) - dist;

    // 정규화된 방향 벡터
    dx /= dist;
    dy /= dist;

    // 서로 반씩 밀어내기
    _pos.x += dx * (overlap / 2.0f);
    _pos.y += dy * (overlap / 2.0f);

    other->SetPos({
           otherPos.x - dx * (overlap / 2.0f),
           otherPos.y - dy * (overlap / 2.0f)
        });

    _pos.x = std::clamp(_pos.x, (float)(gBackgroundRect.left), (float)(gBackgroundRect.right));
    _pos.y = std::clamp(_pos.y, (float)(gBackgroundRect.top), (float)(gBackgroundRect.bottom));

    _prevPos = _pos;
    g_framework->SendMovePacket(shared_from_this(), true);
    g_framework->SendMovePacket(other, true);

    _isCollision = false;
}