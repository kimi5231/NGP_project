#pragma once

class GameObject;
class StateMachine;

class State {
public:
	virtual void Enter(GameObject* self) = 0;
	virtual void Exit(GameObject* self) = 0;
	virtual void Tick(GameObject* self, GameObject* other) = 0;
};

class IdleState : public State {
public:
	virtual void Enter(GameObject* self) override {};
	virtual void Exit(GameObject* self) override {};
	virtual void Tick(GameObject* self, GameObject* other) override {};
};

class MoveState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class MoveToTargetState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

// random or player.pos
class SetTargetState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class DeadState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class UseItemState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class UseSkillState : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class StateMachine {
public:
	StateMachine(GameObject* object, State* state = new SetTargetState);
	~StateMachine();
	void Start();
	void Update(GameObject* other = nullptr);

	void ChangeState(State* state);
private:
	GameObject* _object{};
	State* _curState{};
};