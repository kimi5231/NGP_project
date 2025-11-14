#pragma once
#include "Scene.h"
#include "UI.h"
#include "Constant.h"

class GameObject;
class Monster;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	virtual void Update();
	virtual void Render(HDC hdc);
	void AddObject(GameObject*) override;
	void InitObstalce();
public:
	virtual void ProcessInput() override;
private:
	std::vector<PlayerRef> _players;
	std::vector<MonsterRef> _monsters;
	std::vector<GameObjectRef> _objects;	// obstacle, bomb, projectile 등
	//std::shared_ptr<Merchant> _merchant;

	// UI
	std::vector<std::shared_ptr<UI>> _ui;
	ProgressBar _timerUI{ Vertex{ FRAME_BUFFER_WIDTH / 2, 30}, Vertex{500, 20}, GAME_TIME };
	float _stagetime{};
	int _curStage{1};
};