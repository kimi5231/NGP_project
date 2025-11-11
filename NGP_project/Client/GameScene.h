#pragma once
#include "Scene.h"

class GameObject;
class Monster;
class Merchant;
class UI;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	virtual void Update();
	virtual void Render(HDC hdc);
	void AddObject(GameObject*) override;
public:
	virtual void ProcessInput() override;
private:
	std::vector<PlayerRef> _players;
	std::vector<MonsterRef> _monsters;
	std::vector<GameObjectRef> _objects;	// obstacle, bomb, projectile 등
	//std::shared_ptr<Merchant> _merchant;
	std::vector<std::shared_ptr<UI>> _ui;
};