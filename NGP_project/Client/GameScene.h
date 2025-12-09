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

	void CreateObstacle();

	// Add Object
	void AddPlayer(int id, PlayerRef player);
	void AddMonster(int id, MonsterRef monster);
	void AddObject(int id, GameObjectRef object);

	void AddEndGameUi(bool isStay, Vertex center, Vertex size, std::wstring text);	// EndGame 패킷 받으면 생성할 UI

	// Get Object
	PlayerRef GetLocalPlayer() const { return _localPlayer; }
	MonsterRef GetMonster(int id) { return _monsters[id]; }
	GameObjectRef GetGameObject(int id){ return _objects[id]; }
	std::unordered_map<int, PlayerRef> GetPlayers() { return _players; }
	std::unordered_map<int, MonsterRef> GetMonsters() { return _monsters; }
	std::unordered_map<int, GameObjectRef> GetObjects() { return _objects; }

	// Remove Object
	void RemovePlayer(int id); 
	void RemoveMonster(int id); 
	void RemoveObject(int id); 

	void SetGameNetwork(GameNetwork* gameNetwork) { _gameNetwork = gameNetwork; }

	// Input
	Dir ConvertVecToDir(const Vertex& dir);		// Vec -> Dir

	// Sync
	void SyncPlayer(int id, const Vertex& pos, const Dir dir, const ObjectState state);
	void SyncMonster(int id, const Vertex& pos, const Dir dir, const ObjectState state);
	void SyncObject(int id, const Vertex& pos);
	void GetItemLocalPlayer(ItemType itemType);

	void SyncObjectTimer(const int timer, const int id);

	void SetPlayerState(int id, const ObjectState state);
	void SetMonsterState(int id, const ObjectState state);

	void SetLifeOfLocalPlayer(int life);

	void SetObjectState(int id, const ObjectState state);

	// Get UI
	ProgressBar& GetTimerUI() { return _timerUI; }
	
	void ShotGunFire(Dir baseDir);
public:
	virtual void ProcessInput() override;

private:
	PlayerRef _localPlayer;	// MyPlayer

	std::unordered_map<int, PlayerRef> _players;	// Other Player(local과 분리)
	std::unordered_map<int, MonsterRef> _monsters;
	std::unordered_map<int, GameObjectRef> _objects; // obstacle, bomb, projectile 등
	std::vector<GameObjectRef> _obstacles;

	// UI
	std::vector<std::shared_ptr<UI>> _ui;
	ProgressBar _timerUI{ Vertex{ 650, 30}, Vertex{1000, 20}, GAME_TIME };
	Button* _stayButton{};
	Button* _leaveButton{};
	bool _isStayButtonActive = true;

	float _stagetime{};
	int _curStage{1};

	GameNetwork* _gameNetwork{};
};