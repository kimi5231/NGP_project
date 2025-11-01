#pragma once

class GameObject;
class StateMachine;

class State {
public:
	virtual void Enter(GameObject* object) = 0;
	virtual void Exit(GameObject* object) = 0;
	virtual void Tick(GameObject* object) = 0;
};

class MoveToTarget : public State{
public:
	virtual void Enter(GameObject* object) override;
	virtual void Exit(GameObject* object) override;
	virtual void Tick(GameObject* object) override;
};

// random or player.pos
class SetTarget : public State {
public:
	virtual void Enter(GameObject* object) override;
	virtual void Exit(GameObject* object) override;
	virtual void Tick(GameObject* object) override;

	void SearchTarget(Vertex targetPos);
};

class Bomb : public State {
public:
	virtual void Enter(GameObject* object) override;
	virtual void Exit(GameObject* object) override;
	virtual void Tick(GameObject* object) override;
};

class Dead : public State {
public:
	virtual void Enter(GameObject* object) override;
	virtual void Exit(GameObject* object) override;
	virtual void Tick(GameObject* object) override;
};

class UseItem : public State {
public:
	virtual void Enter(GameObject* object) override;
	virtual void Exit(GameObject* object) override;
	virtual void Tick(GameObject* object) override;
};

class StateMachine {
public:
	StateMachine(GameObject* object, State* state = new SetTarget);
	~StateMachine();
	void Start();
	void Update();

	void ChangeState(State* state);
private:
	GameObject* _object{};
	State* _curState{};
};