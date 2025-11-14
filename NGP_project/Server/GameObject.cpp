#include "pch.h"
#include "GameObject.h"

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

    _prevPos = pos;
    _pos = pos;
}