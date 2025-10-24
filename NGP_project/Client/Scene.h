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
	virtual void Render() {};
	virtual void Render(HDC) {};
	virtual void ProcessInput(InputManager* inputManager) {};
};