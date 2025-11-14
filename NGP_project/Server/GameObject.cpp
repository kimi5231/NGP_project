#include "pch.h"
#include "GameObject.h"

#define DIFF 5  // 임시 상수

GameObject::GameObject()
    : _state{ ObjectState::Idle }
{
}

GameObject::GameObject(ObjectState state)
    : _state{ state }
{
}

void GameObject::SetPos(Vertex pos)
{
    // speed + a 차이 안나면 셋
    if (_status._speed + DIFF>= abs(_prevPos.x - _pos.x) && _status._speed + DIFF >= abs(_prevPos.x - _pos.x)) {
        _prevPos = _pos;
        _pos = pos;
    }
}