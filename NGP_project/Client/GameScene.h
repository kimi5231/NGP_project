﻿#pragma once
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
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hDC);

public:
	virtual void ProcessInput() override;
private:
	std::vector<PlayerRef> _players;
	std::vector<GameObject*> _objects;
	Monster* _monster;
	Merchant* _merchant;
};