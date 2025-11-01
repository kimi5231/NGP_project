#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"

// MoveToTarget
void MoveToTarget::Enter(GameObject* self)
{
}

void MoveToTarget::Exit(GameObject* self)
{
	// if hp == 0
	/*object->GetStateMachine()->ChangeState(new Dead);*/
	
	self->GetStateMachine()->ChangeState(new SetTarget);
	self->GetStateMachine()->Start();
}

void MoveToTarget::Tick(GameObject* self, GameObject* other)
{
	self->Move();
	if (self->IsArrive()) {
		Exit(self);
	}
}

// SetTarget
void SetTarget::Enter(GameObject* self)
{

}

void SetTarget::Exit(GameObject* self)
{
	self->GetStateMachine()->ChangeState(new MoveToTarget);
	self->GetStateMachine()->Start();
}

void SetTarget::Tick(GameObject* self, GameObject* other)
{
	self->FindTarget(other);
	Exit(self);
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
	if (!_curState) {
		delete _curState;
	}
}

void StateMachine::Start()
{
	_curState->Enter(_object);
}

void StateMachine::Update(GameObject* other)
{
	_curState->Tick(_object, other);
}

void StateMachine::ChangeState(State* state)
{
	if (_curState) {
		delete _curState;
	}

	_curState = state;
}
