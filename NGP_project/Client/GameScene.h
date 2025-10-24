#pragma once
#include "Scene.h"

class GameObject;
class Monster;

class GameScene : public Scene
{
public:
	GameScene() {};
	GameScene(HINSTANCE hInst);
	virtual ~GameScene();

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	void Render(HDC hDC);
private:
	std::vector<PlayerRef> _players;
	std::vector<GameObject*> _objects;
	Monster* _monster;
};