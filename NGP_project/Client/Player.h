#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(HINSTANCE hInst);

	virtual void init(HINSTANCE hInst) override;
private:

};

