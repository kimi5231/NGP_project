#pragma once

class GameObject;
class StateMachine;

class State {
public:
	virtual void Enter(GameObject* self) = 0;
	virtual void Exit(GameObject* self) = 0;
	virtual void Tick(GameObject* self, GameObject* other) = 0;
};

class MoveToTarget : public State{
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

// random or player.pos
class SetTarget : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class Bomb : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class Dead : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class UseItem : public State {
public:
	virtual void Enter(GameObject* self) override;
	virtual void Exit(GameObject* self) override;
	virtual void Tick(GameObject* self, GameObject* other) override;
};

class StateMachine {
public:
	StateMachine(GameObject* object, State* state = new SetTarget);
	~StateMachine();
	void Start();
	void Update(GameObject* other);

	void ChangeState(State* state);
private:
	GameObject* _object{};
	State* _curState{};
};