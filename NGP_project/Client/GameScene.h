#pragma once
#include "Scene.h"

class Player;

class GameScene : public Scene
{
public:
	GameScene() {};
	virtual ~GameScene() {};

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();

private:
	std::vector<PlayerRef> _players;
};