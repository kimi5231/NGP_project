#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
	bool Move() override;
	bool _isBomb{};
};
