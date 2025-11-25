#pragma once
#include "Scene.h"
#include "UI.h"
#include "Constant.h"

class Player;
class Monster;
class GameObject;
class GameNetwork;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	virtual void Update();
	virtual void Render(HDC hdc);

	// Add Object
	void AddPlayer(int id, PlayerRef player);
	void AddMonster(int id, MonsterRef monster);
	void AddObject(int id, GameObjectRef object);

	// Get Object
	PlayerRef GetLocalPlayer() const { return _localPlayer; }
	MonsterRef GetMonster(int id) { return _monsters[id]; }
	GameObjectRef GetGameObject(int id){ return _objects[id]; }
	std::unordered_map<int, PlayerRef> GetPlayers() { return _players; }
	std::unordered_map<int, MonsterRef> GetMonsters() { return _monsters; }
	std::unordered_map<int, GameObjectRef> GetObjects() { return _objects; }

	// Remove Object
	void RemovePlayer(int id) { _players.erase(id); }
	void RemoveMonster(int id) { _monsters.erase(id); }
	void RemoveObject(int id) { _objects.erase(id); }

	void SetGameNetwork(GameNetwork* gameNetwork) { _gameNetwork = gameNetwork; }

	// Input
	Dir ConvertVecToDir(const Vertex& dir);		// Vec -> Dir

	// Sync
	void SyncPlayer(int id, const Vertex& pos, const Dir dir, const ObjectState state);

	void SyncMonsterPos(int id, const Vertex& pos);
	void SyncMonsterDir(int id, const Dir dir);

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

	GameNetwork* _gameNetwork{};
};