#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"

// Move
void MoveState::Enter(GameObject* self)
{
}

void MoveState::Exit(GameObject* self)
{
	self->GetStateMachine()->ChangeState(new DeadState);
	self->GetStateMachine()->Start();
}

void MoveState::Tick(GameObject* self, GameObject* other)
{
	self->Move();
	Vertex pos = self->GetPos();
	if (pos.x < 0 || pos.x > FRAME_BUFFER_WIDTH ||
		pos.y < 0 || pos.y > FRAME_BUFFER_HEIGHT) {
		Exit(self);
	}
}

// MoveToTarget
void MoveToTarget::Enter(GameObject* self)
{
}

void MoveToTarget::Exit(GameObject* self)
{
	// if hp == 0
	/*object->GetStateMachine()->ChangeState(new Dead);*/
	
	self->GetStateMachine()->ChangeState(new SetTargetState);
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
void SetTargetState::Enter(GameObject* self)
{

}

void SetTargetState::Exit(GameObject* self)
{
	self->GetStateMachine()->ChangeState(new MoveToTarget);
	self->GetStateMachine()->Start();
}

void SetTargetState::Tick(GameObject* self, GameObject* other)
{
	self->FindTarget(other);
	Exit(self);
}


// Bomb
void BombState::Enter(GameObject* self)
{
}

void BombState::Exit(GameObject* self)
{
}

void BombState::Tick(GameObject* self, GameObject* other)
{
}

// Dead
void DeadState::Enter(GameObject* self)
{
}

void DeadState::Exit(GameObject* self)
{
}

void DeadState::Tick(GameObject* self, GameObject* other)
{
	self->SetToDead();
	Exit(self);
}

// UseItem
void UseItemState::Enter(GameObject* self)
{
}

void UseItemState::Exit(GameObject* self)
{
}

void UseItemState::Tick(GameObject* self, GameObject* other)
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

void StateMachine::Update()
{
	_curState->Tick(_object, nullptr);
}

void StateMachine::ChangeState(State* state)
{
	if (_curState) {
		delete _curState;
	}

	_curState = state;
}
