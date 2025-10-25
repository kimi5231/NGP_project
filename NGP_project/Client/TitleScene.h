#pragma once
#include "Scene.h"

class GameObject;

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

public:
	virtual void Update() {};
	virtual void Render(HDC hdc);

private:
	virtual void ProcessInput() {};
};