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
void MoveToTargetState::Enter(GameObject* self)
{
}

void MoveToTargetState::Exit(GameObject* self)
{
}

void MoveToTargetState::Tick(GameObject* self, GameObject* other)
{
}

// SetTarget
void SetTargetState::Enter(GameObject* self)
{

}

void SetTargetState::Exit(GameObject* self)
{
}

void SetTargetState::Tick(GameObject* self, GameObject* other)
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
}

// UseItem - item 효과 발동 이후 바로 종료
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
}

void UseSkillState::Tick(GameObject* self, GameObject* other)
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

void StateMachine::ChangeState(State* state)
{
}
