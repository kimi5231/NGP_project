#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
    : _stateMachine{ new StateMachine{this} }
{
}