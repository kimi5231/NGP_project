#include "pch.h"
#include "BombObject.h"
#include "Constant.h"
#include "TimeManager.h"

BombObject::BombObject()
{   
    _type = ObjectType::Bomb;
    _maxCnt = 6;
}

BombObject::BombObject(Vertex pos)
{
    _pos = pos;
    _type = ObjectType::Bomb;
    _maxCnt = 6;
}

void BombObject::Update()
{
    // 시간 지남에 따라 상태 변화하도록 변경하기
    if (GET_SINGLE(TimeManager)->CheckTimer(_timer, (float)BOMB_TIME / _maxCnt)) {
        // 시간 send
    }
    
    if (_timer >= BOMB_TIME) {
        if (!_isBomb) {
            _isBomb = true;
            _maxCnt = 7;
            _size.x *= 3;
            _size.y *= 3;
        }
        else {
            SetState(ObjectState::Dead);
            // 상태 send
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

    _pos.x = std::clamp(_pos.x, (float)(gBackgroundRect.left), (float)(gBackgroundRect.right));
    _pos.y = std::clamp(_pos.y, (float)(gBackgroundRect.top), (float)(gBackgroundRect.bottom));

    return true;
}
