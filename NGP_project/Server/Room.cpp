#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "TimeManager.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"
#include "Projectile.h"
#include "Item.h"
#include "BombObject.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

Room::Room()
{
	InitializeCriticalSection(&_objectCS);

	_generateID = 1;
	_playerCount = 0;
	_monsterCount = 0;
	_state = RoomState::Idle;
	_timer = 50;
}

Room::~Room()
{
	DeleteCriticalSection(&_objectCS);
}

void Room::Update()
{
	if (_state != RoomState::Playing)
		return;

	// Timer Update
	static float sumTime;
	sumTime += GET_SINGLE(TimeManager)->GetDeltaTime();
	if (sumTime > 1)
	{
		sumTime = 0;
		_timer -= 1;
		g_framework->SendUpdateTimerPacket(true);
	}
	
	SpawnMonster();

	EnterCriticalSection(&_objectCS);
	for (const auto& item : _players)
		item.second->Update();
	for (const auto& item : _monsters)
		item.second->Update();
	for (const auto& item : _items)
		item.second->Update();
	for (const auto& item : _projectiles)
		item.second->Update();
	for (const auto& item : _bombs) {
		item.second->Update();
		if (item.second->IsState(ObjectState::Dead)) {
			g_framework->AddRemoveObject(item.second);
		}
	}
	LeaveCriticalSection(&_objectCS);

	EnterCriticalSection(&_objectCS);
	// Player 충돌 처리
	for (const auto& playerItem : _players)
	{
		// Monster와 충돌 처리
		for (const auto& monsterItem : _monsters)
		{
			if (monsterItem.second->IsCollision(playerItem.second))
				playerItem.second->SetState(ObjectState::Dead);
		}

		// Item과 충돌 처리
		for (const auto& itemItem : _items)
		{
			if (itemItem.second->IsCollision(playerItem.second))
			{

			}
		}
	}
	LeaveCriticalSection(&_objectCS);

	EnterCriticalSection(&_objectCS);
	// Monster 충돌 처리
	for (const auto& monsterItem : _monsters)
	{
		// Projectile과 충돌 처리
		for (const auto& projectileItem : _projectiles)
		{
			if (projectileItem.second->IsCollision(monsterItem.second))
			{

			}
		}

		// Monster와 충돌 처리
		for (const auto& monsterItem2 : _monsters)
		{
			if (monsterItem2.first == monsterItem.first)
				continue;

			if (monsterItem2.second->IsCollision(monsterItem.second))
			{

			}
		}
	}
	LeaveCriticalSection(&_objectCS);
}

GameObjectRef Room::AddObject(ObjectType type, Vertex pos, Dir dir)
{
	GameObjectRef object;

	EnterCriticalSection(&_objectCS);
	switch (type)
	{
	case ObjectType::Player:
		_players[_generateID] = std::make_shared<Player>();
		object = _players[_generateID];
		_playerCount++;
		break;
	case ObjectType::NormalMonster:
		_monsters[_generateID] = std::make_shared<NormalMonster>();
		object = _monsters[_generateID];
		_monsterCount++;
		std::dynamic_pointer_cast<Monster>(object)->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::TankMonster:
		_monsters[_generateID] = std::make_shared<TankMonster>();
		object = _monsters[_generateID];
		_monsterCount++;
		std::dynamic_pointer_cast<Monster>(object)->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::BomberMonster:
		_monsters[_generateID] = std::make_shared<BomberMonster>();
		object = _monsters[_generateID];
		_monsterCount++;
		std::dynamic_pointer_cast<Monster>(object)->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::RespawnMonster:
		_monsters[_generateID] = std::make_shared<RespawnMonster>();
		object = _monsters[_generateID];
		_monsterCount++;
		std::dynamic_pointer_cast<Monster>(object)->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::ObstacleMonster:
		_monsters[_generateID] = std::make_shared<ObstacleMonster>();
		object = _monsters[_generateID];
		_monsterCount++;
		std::dynamic_pointer_cast<Monster>(object)->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::Item:
		break;
	case ObjectType::Bullet:
		_projectiles[_generateID] = std::make_shared<Projectile>(dir, pos);
		object = _projectiles[_generateID];
		break;
	case ObjectType::Bomb:
		_bombs[_generateID] = std::make_shared<BombObject>(pos);
		object = _bombs[_generateID];
		break;
	case ObjectType::Obstacle:
		break;
	}
	LeaveCriticalSection(&_objectCS);

	object->SetID(_generateID++);

	g_framework->SendAddObjectPacket(object, true);

	return object;
}

void Room::RemoveObject(ObjectType type, int id)
{
	GameObjectRef object;

	EnterCriticalSection(&_objectCS);
	switch (type)
	{
	case ObjectType::Player:
		object = _players[id];
		_players.erase(id);
		_playerCount--;
		break;
	case ObjectType::NormalMonster:
	case ObjectType::TankMonster:
	case ObjectType::BomberMonster:
	case ObjectType::RespawnMonster:
	case ObjectType::ObstacleMonster:
		object = _monsters[id];
		_monsters.erase(id);
		_monsterCount--;
		break;
	case ObjectType::Item:
		object = _items[id];
		_items.erase(id);
		break;
	case ObjectType::Bullet:
		object = _projectiles[id];
		_projectiles.erase(id);
		break;
	case ObjectType::Bomb:
		object = _bombs[id];
		_bombs.erase(id);
		break;
	case ObjectType::Obstacle:
		break;
	}
	LeaveCriticalSection(&_objectCS);

	g_framework->SendRemoveObjectPacket(object, true);
}

GameObjectRef Room::GetObject(ObjectType type, int id)
{
	switch (type)
	{
	case ObjectType::Player:
		return _players[id];
	case ObjectType::NormalMonster:
	case ObjectType::TankMonster:
	case ObjectType::BomberMonster:
	case ObjectType::RespawnMonster:
	case ObjectType::ObstacleMonster:
		return _monsters[id];
	case ObjectType::Item:
		return _items[id];
	case ObjectType::Bullet:
		return _projectiles[id];
	case ObjectType::Bomb:
		break;
	case ObjectType::Obstacle:
		break;
	}
}

void Room::SpawnMonster()
{
	// 시간에 따라 몬스터 추가
	static float monsterSpawnTimer{};

	if (GET_SINGLE(TimeManager)->CheckTimer(monsterSpawnTimer, MONSTER_SPAWN_TIME))
	{
		int type = static_cast<int>(ObjectType::NormalMonster) + rand() % 5;
		GameObjectRef monster = AddObject(static_cast<ObjectType>(type));
		if (!monster) 
			return;
	}
}