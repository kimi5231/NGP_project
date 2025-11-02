#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"


// Move
void MoveState::Enter(GameObject* self)
{
}

void MoveState::Exit(GameObject* self)
{
}

void MoveState::Tick(GameObject* self, GameObject* other)
{
}

// MoveToTarget
void MoveToTarget::Enter(GameObject* self)
{
}

void MoveToTarget::Exit(GameObject* self)
{
}

void MoveToTarget::Tick(GameObject* self, GameObject* other)
{
}

// SetTarget
void SetTarget::Enter(GameObject* self)
{

}

void SetTarget::Exit(GameObject* self)
{
}

void SetTarget::Tick(GameObject* self, GameObject* other)
{
}


// Bomb
void Bomb::Enter(GameObject* self)
{
}

void Bomb::Exit(GameObject* self)
{
}

void Bomb::Tick(GameObject* self, GameObject* other)
{
}

// Dead
void Dead::Enter(GameObject* self)
{
}

void Dead::Exit(GameObject* self)
{
}

void Dead::Tick(GameObject* self, GameObject* other)
{
}

// UseItem
void UseItem::Enter(GameObject* self)
{
}

void UseItem::Exit(GameObject* self)
{
}

void UseItem::Tick(GameObject* self, GameObject* other)
{
}

// StateMachine
StateMachine::StateMachine(GameObject* object, State* state)
	: _object{ object }, _curState{ state }
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::Start()
{
}

void StateMachine::Update(GameObject* other)
{
}

void StateMachine::Update()
{
}

void StateMachine::ChangeState(State* state)
{
}
