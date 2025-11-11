#pragma once
#include "BoundingBox.h"

class UI {
public:
	UI(const Vertex& center, const Vertex& size, const DWORD color = {}, const std::wstring& text = {}, const bool isNumtext = false)
		: _box{ BoundingBox{ center, size } }, _text{ text }, _penColor{ color }, _brushColor{ color }, _isNumText{ isNumtext }
	{
	}
	UI(const Vertex& center, const Vertex& size, const std::wstring& text = {}, const bool isNumtext = false)
		: _box{ BoundingBox{ center, size } }, _text{ text }, _isNumText{ isNumtext }
	{
	}

	bool Intersects(const POINT& point) const { return _box.Intersects(point); }
	void Render(HDC hdc, HDC srcDC, int num);
	ObjectType GetObjectType() const { return _type; }
protected:
	BoundingBox _box;
	ObjectType _type{ ObjectType::UI};

	std::wstring _text{ };	// UI에 텍스트가 필요하다면 사용
	DWORD _penColor{RGB(255,255,255)};
	DWORD _brushColor{ RGB(255,255,255) };
	bool _isNumText{ false };
};

class Button : public UI {
public:
	Button(const Vertex& center, const Vertex& size, const DWORD color = {}, const std::wstring& text = {}) : UI(center, size, color, text)
	{
		_type = ObjectType::Button;
	}
	Button(const Vertex& center, const Vertex& size, const std::wstring& text = {}) : UI(center, size, text)
	{
		_type = ObjectType::Button;
		_penColor = {};
		_brushColor = { RGB(50, 50, 50) };
	}
};