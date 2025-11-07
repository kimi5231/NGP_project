#pragma once
#include "GameObject.h"

class Button : public GameObject {
public:
	Button(const Vertex& pos, const Vertex& size)
	{
		_type = ObjectType::Button;
		_pos = pos;
		_size = size;
	}
	void Render(HDC hdc, HDC srcDC) override;
};

