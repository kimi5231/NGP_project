#pragma once

class GameObject;
class Monster;
class StateMachine;

class State {
public:
	virtual void Enter(Monster* self) = 0;
	virtual void Exit(Monster* self) = 0;
	virtual void Tick(Monster* self) = 0;
};

class IdleState : public State{
public:
	virtual void Enter(Monster* self) override {};
	virtual void Exit(Monster* self) override {};
	virtual void Tick(Monster* self) override {};
};

class MoveToTargetState : public State{
public:
	virtual void Enter(Monster* self) override;
	virtual void Exit(Monster* self) override;
	virtual void Tick(Monster* self) override;
};

// random or player.pos
class FindTargetState : public State {
public:
	virtual void Enter(Monster* self) override;
	virtual void Exit(Monster* self) override;
	virtual void Tick(Monster* self) override;
};

class DeadState : public State {
public:
	virtual void Enter(Monster* self) override;
	virtual void Exit(Monster* self) override;
	virtual void Tick(Monster* self) override;
};

class UseSkillState : public State {
public:
	virtual void Enter(Monster* self) override;
	virtual void Exit(Monster* self) override;
	virtual void Tick(Monster* self) override;
};

class StateMachine {
public:
	StateMachine(Monster* object, State* state = new FindTargetState);
	StateMachine(Monster* object, ObjectState state);
	~StateMachine();
	void Start();
	void Update();

	void ChangeState(State* state);
private:
	Monster* _object{};
	State* _curState{};
};