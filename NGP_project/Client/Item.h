#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item() {};
	virtual ~Item() {};

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();

private:

};