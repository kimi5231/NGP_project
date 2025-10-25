#pragma once

class GameObject;

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

public:
	virtual void Update();
	virtual void Render(HDC hdc);

public:
	virtual void ProcessInput() {};

protected:
	std::vector<GameObjectRef> _objects;

protected:
	// Double Buffering
	HDC _memDC{};
	HDC _memDCImage{};

	HBITMAP _hbit{}; 
	HBITMAP _oldbit{};
};