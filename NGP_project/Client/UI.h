#pragma once
#include "BoundingBox.h"

class BoundingBox;

class UI {
public:
	UI(const Vertex& center, const Vertex& size, const std::wstring& text = {}, const DWORD color = RGB(255, 255, 255), const bool isNumtext = false)
		: _box{ BoundingBox{ center, size } }, _text{ text }, _penColor{ color }, _brushColor{ color }, _isNumText{ isNumtext }
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
	Button(const Vertex& center, const Vertex& size, const std::wstring& text = {}, const DWORD color = RGB(50, 50, 50))
		: UI(center, size, text, color)
	{
		_type = ObjectType::Button;
	}
};

class ProgressBar : public UI {
public:
	ProgressBar(const Vertex& center, const Vertex& size, UINT maxProgress)
		: UI(center, size), _progress{maxProgress}, _maxProgress{ maxProgress }
	{
		_text = L"time: ";
		_brushColor = RGB(0, 255, 0);
		_isNumText = true;
		_maxSize = _box._halfSize.x;
	}

	void Update(float& currentTime);
private:
	UINT _maxProgress, _progress;
	int _maxSize;
};