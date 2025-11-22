#pragma once
#include "Scene.h"
#include "UI.h"
#include "Constant.h"

class Player;
class Monster;
class GameObject;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	virtual void Update();
	virtual void Render(HDC hdc);

	void AddPlayer(int id, PlayerRef player);
	void AddMonster(int id, MonsterRef monster);
	void AddObject(int id, GameObjectRef object);

	PlayerRef GetLocalPlayer() const { return _localPlayer; }
	MonsterRef GetMonster(int id) { return _monsters[id]; }
	GameObjectRef GetGameObject(int id){ return _objects[id]; }

	void InitObstalce();
	void InitStage();
	void SpawnMonster();

public:
	virtual void ProcessInput() override;

private:
	PlayerRef _localPlayer;	// MyPlayer

	std::unordered_map<int, PlayerRef> _players;
	std::unordered_map<int, MonsterRef> _monsters;
	std::unordered_map<int, GameObjectRef> _objects; // obstacle, bomb, projectile 등

	// UI
	std::vector<std::shared_ptr<UI>> _ui;
	ProgressBar _timerUI{ Vertex{ FRAME_BUFFER_WIDTH / 2, 30}, Vertex{500, 20}, GAME_TIME };
	float _stagetime{};
	int _curStage{1};
};