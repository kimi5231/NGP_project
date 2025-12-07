#include "pch.h"
#include "BombObject.h"
#include "Constant.h"
#include "TimeManager.h"
#include "Global.h"
#include "ServerFramework.h"

BombObject::BombObject()
{   
    _type = ObjectType::Bomb;
    _maxCnt = 6;
    _state = ObjectState::Idle;
}

BombObject::BombObject(Vertex pos)
{
    _pos = pos;
    _type = ObjectType::Bomb;
    _maxCnt = 6;
    _state = ObjectState::Idle;
}

void BombObject::Update()
{
    if (_state == ObjectState::Move) {
        Move();
    }

    // 시간 지남에 따라 상태 변화하도록 변경하기
    if (GET_SINGLE(TimeManager)->CheckTimer(_timerOffset, BOMB_TIME / _maxCnt)) {
        // 시간 send
        _timer++;
        g_framework->SendUpdateTimerPacket(shared_from_this(), true);   // 오브젝트 보내기 필요
    }
    
    if (_timer >= _maxCnt) {
        if (!_isBomb) {
            _isBomb = true;
            _maxCnt = 7;
            _size.x *= 3;
            _size.y *= 3;
            _timer = 0;
            g_framework->SendUpdateTimerPacket(shared_from_this(), true);   // 오브젝트 보내기 필요
        }
        else {
            g_framework->AddRemoveObject(shared_from_this());
        }
    }
}

bool BombObject::Move()
{
    _prevPos = _pos;
    switch (_dir)
    {
    case Dir::Left:
        _pos.x -= CELL_SIZE;
        break;
    case Dir::Right:
        _pos.x += CELL_SIZE;
        break;
    case Dir::Up:
        _pos.y -= CELL_SIZE;
        break;
    case Dir::Down:
        _pos.y += CELL_SIZE;
        break;
    }

    int sizeOffset = CELL_SIZE / 2;

    float minX = gBackgroundRect.left + sizeOffset + CELL_SIZE - 5;                      // 벽 오른쪽 한 칸
    float maxX = gBackgroundRect.left + sizeOffset + (BOARD_SIZE - 2) * CELL_SIZE + 5;   // 벽 왼쪽 한 칸 내부

    float minY = gBackgroundRect.top + sizeOffset + CELL_SIZE;                       // 위쪽 벽 아래 한 칸
    float maxY = gBackgroundRect.top + sizeOffset + (BOARD_SIZE - 2) * CELL_SIZE;    // 아래쪽 벽 위 한 칸

    _pos.x = std::clamp(_pos.x, minX, maxX);
    _pos.y = std::clamp(_pos.y, minY, maxY);

    g_framework->SendMovePacket(shared_from_this(), true);

    return true;
}
