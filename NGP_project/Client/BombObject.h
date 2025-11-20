#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
	void Move() override;
	bool _isBomb{};
};
