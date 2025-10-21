#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene() {};
	GameScene(HINSTANCE hInst);
	virtual ~GameScene() {};

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	void Render(HDC hDC);
private:
	std::vector<PlayerRef> _players;
};