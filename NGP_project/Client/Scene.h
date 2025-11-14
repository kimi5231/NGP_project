#pragma once

class GameObject;

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

public:
	virtual void Update() {};
	virtual void Render(HDC hdc) {};

public:
	virtual void ProcessInput() {};
};