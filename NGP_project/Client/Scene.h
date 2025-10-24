#pragma once

class InputManager;

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Render(HDC hDC) {};

public:
	virtual void ProcessInput(InputManager* inputManager) {};
};