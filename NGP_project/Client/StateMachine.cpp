#include "pch.h"
#include "StateMachine.h"
#include "GameObject.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> randWidth(0, FRAME_BUFFER_WIDTH);
std::uniform_int_distribution<> randHeight(0, FRAME_BUFFER_HEIGHT);

// MoveToTarget
void MoveToTarget::Enter(GameObject* object)
{
}

void MoveToTarget::Exit(GameObject* object)
{
	// if hp == 0
	/*object->GetStateMachine()->ChangeState(new Dead);*/
	
	object->GetStateMachine()->ChangeState(new SetTarget);
	object->GetStateMachine()->Start();
}

void MoveToTarget::Tick(GameObject* object)
{
	object->Move();
	if (object->IsArrive()) {
		Exit(object);
	}
}

// SetTarget
void SetTarget::Enter(GameObject* object)
{

}

void SetTarget::Exit(GameObject* object)
{
	object->GetStateMachine()->ChangeState(new MoveToTarget);
	object->GetStateMachine()->Start();
}

void SetTarget::Tick(GameObject* object)
{	
	object->SetTargetPos({ randWidth(gen), randHeight(gen) });
	Exit(object);
}

void SetTarget::SearchTarget(Vertex targetPos)
{
}

// Bomb
void Bomb::Enter(GameObject* object)
{
}

void Bomb::Exit(GameObject* object)
{
}

void Bomb::Tick(GameObject* object)
{
}

// Dead
void Dead::Enter(GameObject* object)
{
}

void Dead::Exit(GameObject* object)
{
}

void Dead::Tick(GameObject* object)
{
}

// UseItem
void UseItem::Enter(GameObject* object)
{
}

void UseItem::Exit(GameObject* object)
{
}

void UseItem::Tick(GameObject* object)
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

void StateMachine::Update()
{
	_curState->Tick(_object);
}

void StateMachine::ChangeState(State* state)
{
	if (_curState) {
		delete _curState;
	}

	_curState = state;
}
