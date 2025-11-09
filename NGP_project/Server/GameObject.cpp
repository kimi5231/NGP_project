#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
    : _stateMachine{ new StateMachine{this} }, _state{ ObjectState::Alive }
{
}

GameObject::GameObject(State* state)
    : _stateMachine{ new StateMachine{this, state } }, _state{ ObjectState::Alive }
{
}