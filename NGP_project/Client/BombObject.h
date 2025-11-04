#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
	void Render(HDC hdc, HDC srcDC) override;
private:
	bool _isBomb{};
	int _size{ CELL_SIZE };
};
