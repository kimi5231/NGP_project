#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
	bool Move() override;
	bool _isBomb{};

private:
	int _maxCnt;	// 폭탄 터지는 카운트
};
