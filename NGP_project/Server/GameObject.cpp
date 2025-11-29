#include "pch.h"
#include "GameObject.h"

#define DIFF 5.001  // 임시 상수

GameObject::GameObject()
    : _state{ ObjectState::Idle }
{
}

GameObject::GameObject(ObjectState state)
    : _state{ state }
{
}

GameObject::GameObject(ObjectType type, Vertex pos)
    : _type{ type }, _pos{ pos }
{
}

bool GameObject::IsArrive()
{
    if (_status._speed + DIFF >= abs(_targetPos.x - _pos.x) && _status._speed + DIFF >= abs(_targetPos.y - _pos.y)) {
        return true;
    }

    return false;
}

bool GameObject::SetPos(Vertex pos)
{
    // speed + a 차이 안나면 셋
    if (_status._speed + DIFF>= abs(_prevPos.x - _pos.x) && _status._speed + DIFF >= abs(_prevPos.y - _pos.y)) {
        _prevPos = _pos;
        _pos = pos;
        return true;
    }

    return false;
}