#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
public:
	TitleScene() {};
	virtual ~TitleScene() {};

public:
	virtual void Init();
	virtual void Update() {};
	virtual void Render(HDC hDC);

private:
	virtual void ProcessInput() {};
};