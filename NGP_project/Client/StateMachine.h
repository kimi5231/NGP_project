#pragma once

class GameObject;
class StateMachine;

class State {
public:
	virtual void enter(GameObject* object) = 0;
	virtual void exit(GameObject* object) = 0;
	virtual void doing(GameObject* object) = 0;
};

class MoveToTarget : public State{
public:
	virtual void enter(GameObject* object) override;
	virtual void exit(GameObject* object) override;
	virtual void doing(GameObject* object) override;
};

// random or player.pos
class SetTarget : public State {
public:
	virtual void enter(GameObject* object) override;
	virtual void exit(GameObject* object) override;
	virtual void doing(GameObject* object) override;
};

class Bomb : public State {
public:
	virtual void enter(GameObject* object) override;
	virtual void exit(GameObject* object) override;
	virtual void doing(GameObject* object) override;
};

class Dead : public State {
public:
	virtual void enter(GameObject* object) override;
	virtual void exit(GameObject* object) override;
	virtual void doing(GameObject* object) override;
};

class UseItem : public State {
public:
	virtual void enter(GameObject* object) override;
	virtual void exit(GameObject* object) override;
	virtual void doing(GameObject* object) override;
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