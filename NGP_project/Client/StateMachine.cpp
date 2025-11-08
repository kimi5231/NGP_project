#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"
#include "Monster.h"
#include "BombObject.h"

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
void MoveToTargetState::Enter(GameObject* self)
{
}

void MoveToTargetState::Exit(GameObject* self)
{
	// if hp == 0
	/*object->GetStateMachine()->ChangeState(new Dead);*/
	
	self->GetStateMachine()->ChangeState(new SetTargetState);
	self->GetStateMachine()->Start();
}

void MoveToTargetState::Tick(GameObject* self, GameObject* other)
{
	self->Move();
	if (self->IsArrive() || dynamic_cast<Monster*>(self)->GetIsFollow()) {
		Exit(self);
	}
}

// SetTarget
void SetTargetState::Enter(GameObject* self)
{

}

void SetTargetState::Exit(GameObject* self)
{
	self->GetStateMachine()->ChangeState(new UseSkillState);
	self->GetStateMachine()->Start();
}

void SetTargetState::Tick(GameObject* self, GameObject* other)
{
	self->FindTarget(other);
	Exit(self);
}
// Dead
void DeadState::Enter(GameObject* self)
{
	self->SetToDead();
}

void DeadState::Exit(GameObject* self)
{
}

void DeadState::Tick(GameObject* self, GameObject* other)
{
	//Exit(self);
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

// UseSkill
void UseSkillState::Enter(GameObject* self)
{

}

void UseSkillState::Exit(GameObject* self)
{
	self->GetStateMachine()->ChangeState(new MoveToTargetState);
	self->GetStateMachine()->Start();
}

void UseSkillState::Tick(GameObject* self, GameObject* other)
{
	if (dynamic_cast<Monster*>(self)->UseSkill()) return;
	Exit(self);
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