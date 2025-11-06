#pragma once
#include "GameObject.h"

class BombObject : public GameObject {
public:
	BombObject();
	BombObject(Vertex pos);

	void Update() override;
	void Render(HDC hdc, HDC srcDC) override;
private:
	size_t _bombCount{};
	bool _isBomb{};
	int _size{ CELL_SIZE };
};
