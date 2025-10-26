#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> randWidth(0, FRAME_BUFFER_WIDTH);
std::uniform_int_distribution<> randHeight(0, FRAME_BUFFER_HEIGHT);

// MoveToTarget
void MoveToTarget::enter(GameObject* object)
{
}

void MoveToTarget::exit(GameObject* object)
{
	object->GetStateMachine()->ChangeState(new Dead);
	object->GetStateMachine()->Start();
}

void MoveToTarget::doing(GameObject* object)
{
	object->Move();
}

// SetTarget
void SetTarget::enter(GameObject* object)
{

}

void SetTarget::exit(GameObject* object)
{
	object->GetStateMachine()->ChangeState(new MoveToTarget);
	object->GetStateMachine()->Start();
}

void SetTarget::doing(GameObject* object)
{	
	object->SetTargetPos({ randWidth(gen), randHeight(gen) });
	exit(object);
}

// Bomb
void Bomb::enter(GameObject* object)
{
}

void Bomb::exit(GameObject* object)
{
}

void Bomb::doing(GameObject* object)
{
}

// Dead
void Dead::enter(GameObject* object)
{
}

void Dead::exit(GameObject* object)
{
}

void Dead::doing(GameObject* object)
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
	_curState->enter(_object);
}

void StateMachine::Update()
{
	_curState->doing(_object);
}

void StateMachine::ChangeState(State* state)
{
	if (_curState) {
		delete _curState;
	}

	_curState = state;
}
