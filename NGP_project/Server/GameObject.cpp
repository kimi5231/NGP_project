#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
    : _stateMachine{ new StateMachine{this} }
{
}

GameObject::GameObject(State* state)
    : _stateMachine{ new StateMachine{this, state } }
{
}