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