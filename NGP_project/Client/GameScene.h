#pragma once
#include "Scene.h"

class GameObject;
class Monster;
class Merchant;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	virtual void Update();
	virtual void Render(HDC hdc);

public:
	virtual void ProcessInput() override;
};