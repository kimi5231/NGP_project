#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
private:
	bool _isBomb{};
};
