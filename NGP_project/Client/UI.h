#pragma once
#include "BoundingBox.h"

class UI {
public:
	UI(const Vertex& center, const Vertex& size) : _box{ BoundingBox{ center, size } } {}

	bool Intersects(const POINT& point) const { return _box.Intersects(point); }
	void Render(HDC hdc, HDC srcDC);
	ObjectType GetObjectType() const { return _type; }
protected:
	BoundingBox _box;
	ObjectType _type{ ObjectType::UI};

	std::wstring _text{ };	// UI에 텍스트가 필요하다면 사용
	DWORD _penColor{};
	DWORD _brushColor{};
};

class Button : public UI {
public:
	Button(const Vertex& center, const Vertex& size, const std::wstring& text = {}) : UI(center, size)
	{
		_type = ObjectType::Button; 
		_penColor = {};
		_brushColor = {RGB(50, 50, 50)};
		_text = text;
	}
};