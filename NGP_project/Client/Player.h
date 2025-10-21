#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	virtual ~Player();

	virtual void Init();
	virtual void Update();
	virtual void Render();

private:
	POINT _pos{};
	int _attack{};
	int _speed{};
	int _life{};
	int _coin{};
	bool _isItem{};
};