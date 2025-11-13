#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"
#include "Monster.h"
#include "BombObject.h"
#include "Global.h"
#include "Player.h"

// MoveToTarget
void MoveToTargetState::Enter(Monster* self)
{
	self->SetState(ObjectState::Move);
}

void MoveToTargetState::Exit(Monster* self)
{	
	self->GetStateMachine()->ChangeState(new FindTargetState);
	self->GetStateMachine()->Start();
}

void MoveToTargetState::Tick(Monster* self, GameObject* other)
{
	self->Move();
	if (self->IsArrive() || self->GetIsFollow()) {
		Exit(self);
	}
}

// SetTarget
void FindTargetState::Enter(Monster* self)
{
	self->SetState(ObjectState::Move);
}

void FindTargetState::Exit(Monster* self)
{
	if (self->CanUseSkill()) {
		self->GetStateMachine()->ChangeState(new UseSkillState);
		self->GetStateMachine()->Start();
	}
	else {
		self->GetStateMachine()->ChangeState(new MoveToTargetState);
		self->GetStateMachine()->Start();
	}
}

void FindTargetState::Tick(Monster* self, GameObject* other)
{
	self->FindTarget(other);
	Exit(self);
}
// Dead
void DeadState::Enter(Monster* self)
{
	self->SetState(ObjectState::Dead);
}

void DeadState::Exit(Monster* self)
{

}

void DeadState::Tick(Monster* self, GameObject* other)
{
	//Exit(self);
}

// UseSkill
void UseSkillState::Enter(Monster* self)
{
	self->SetState(ObjectState::UseSkill);
}

void UseSkillState::Exit(Monster* self)
{
	self->GetStateMachine()->ChangeState(new MoveToTargetState);
	self->GetStateMachine()->Start();
}

void UseSkillState::Tick(Monster* self, GameObject* other)
{
	if (self->UseSkill()) return;
	Exit(self);
}

// StateMachine
StateMachine::StateMachine(Monster* object, State* state)
	: _object{ object }, _curState{ state }
{
}

StateMachine::StateMachine(Monster* object, ObjectState state)
	: _object{ object }
{
	switch (state) {
	case ObjectState::Idle:
		_curState = new IdleState;
		break;
	case ObjectState::Move:
		_curState = new MoveToTargetState;
		break;
	case ObjectState::UseSkill:
		_curState = new UseSkillState;
		break;
	case ObjectState::Dead:
		_curState = new DeadState;
		break;
	default:
		break;
	}
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
	if (_object->IsState(ObjectState::Dead)) {
		ChangeState(new DeadState);
		Start();
	}
}

void StateMachine::ChangeState(State* state)
{
	if (_curState) {
		delete _curState;
	}

	_curState = state;
}