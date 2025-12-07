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
#include "GameObject.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

Room::Room()
{
	_generateID = 1;
	_playerCount = 0;
	_monsterCount = 0;
	_state = RoomState::Idle;
	_timer = 50;
}

Room::~Room()
{
}

void Room::Update()
{
	EnterCriticalSection(&g_objectCS);
	for (const auto& item : _players)
		item.second->Update();
	for (const auto& item : _monsters)
		item.second->Update();
	for (const auto& item : _items)
		item.second->Update();
	for (const auto& item : _projectiles)
		item.second->Update();
	for (const auto& item : _bombs)
		item.second->Update();
	LeaveCriticalSection(&g_objectCS);

	if (_state != RoomState::Playing)
		return;

	// Timer Update
	static float sumTime = 1;
	sumTime += GET_SINGLE(TimeManager)->GetDeltaTime();
	if (sumTime > 1)
	{
		g_framework->SendUpdateTimerPacket(true);
		sumTime = 0;
		_timer -= 1;
	}
	
	// 시간이 남았을 때만 몬스터 생성
	if (_timer >= 0)
	{
		SpawnMonster();
	}
	else
	{
		if (_monsterCount == 0)
			ChangeNextStage();
	}


	EnterCriticalSection(&g_objectCS);
	// Player 충돌 처리
	for (const auto& playerItem : _players)
	{
		// Monster와 충돌 처리
		for (const auto& monsterItem : _monsters)
		{
			if (monsterItem.second->IsCollision(playerItem.second)) {
				playerItem.second->Damaged(monsterItem.second->GetDamage());
			}
		}
		// bomb과 충돌 처리
		for (const auto& bombItem : _bombs)
		{
			// 폭발하는 상태에서만 충돌처리
			if (bombItem.second->_isBomb && playerItem.second->IsCollision(bombItem.second))
			{
				playerItem.second->Damaged(bombItem.second->GetDamage());
			}
		}

		// Item과 충돌 처리
		for (const auto& itemItem : _items)
		{
			if (itemItem.second->IsCollision(playerItem.second)) {
				g_framework->SendGetItemPacket(itemItem.second, playerItem.second);
				playerItem.second->SetItem(itemItem.second);
				itemItem.second->SetState(ObjectState::Dead);
			}
		}

		// Obstacle과 충돌 처리
		for (const auto& ObstacleItem : _obstacles)
		{
			if (playerItem.second->IsCollision(ObstacleItem.second))
			{
				playerItem.second->UndoPos();
			}
		}
	}
	LeaveCriticalSection(&g_objectCS);

	EnterCriticalSection(&g_objectCS);
	// Monster 충돌 처리
	for (const auto& monsterItem : _monsters)
	{
		// Projectile과 충돌 처리
		for (const auto& projectileItem : _projectiles)
		{
			if (projectileItem.second->IsCollision(monsterItem.second) && !monsterItem.second->IsState(ObjectState::Dead) && monsterItem.second->CanDamage())
			{
				monsterItem.second->Damaged(projectileItem.second->GetDamage());
				projectileItem.second->SetState(ObjectState::Dead);
			}
		}

		// bomb과 충돌 처리
		for (const auto& bombItem : _bombs)
		{
			// 폭발하는 상태에서만 충돌처리
			if (bombItem.second->_isBomb && monsterItem.second->IsCollision(bombItem.second))
			{
				monsterItem.second->Damaged(bombItem.second->GetDamage());
			}
		}

		// Monster와 충돌 처리
		for (const auto& monsterItem2 : _monsters)
		{
			if (monsterItem2.first == monsterItem.first)
				continue;

			if (monsterItem2.second->IsCollision(monsterItem.second))
			{
				monsterItem2.second->UndoPos();
			}
		}

		// Obstacle과 충돌 처리
		for (const auto& ObstacleItem : _obstacles)
		{
			if (monsterItem.second->IsCollision(ObstacleItem.second))
			{
				monsterItem.second->UndoPos();
			}
			//폭탄/장애물 몬스터는 장애물에 걸리면 경로 다시 탐색
			ObjectType monsterType = monsterItem.second->GetObjectType();
			if (monsterType == ObjectType::BomberMonster || monsterType == ObjectType::ObstacleMonster) {
				monsterItem.second->FindTarget(nullptr);
			}
		}
	}
	LeaveCriticalSection(&g_objectCS);
}

GameObjectRef Room::AddObject(ObjectType type, Vertex pos, Dir dir)
{
	GameObjectRef object;

	EnterCriticalSection(&g_objectCS);
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
		_items[_generateID] = std::make_shared<Item>(pos);
		object = _items[_generateID];
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
		_obstacles[_generateID] = std::make_shared<GameObject>(type, pos);
		object = _obstacles[_generateID];
		break;
	}

	object->SetID(_generateID++);
	LeaveCriticalSection(&g_objectCS);

	g_framework->SendAddObjectPacket(object, true);

	return object;
}

void Room::RemoveObject(ObjectType type, int id)
{
	GameObjectRef object;

	EnterCriticalSection(&g_objectCS);
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
		object = _obstacles[id];
		_obstacles.erase(id);
		break;
	}
	LeaveCriticalSection(&g_objectCS);

	if(object)
		g_framework->SendRemoveObjectPacket(object, true);
}

void Room::ChangeNextStage()
{
	// Stage 클리어
	ClearStage();

	_curStage++;
	_timer = 50;

	int sizeOffset{ CELL_SIZE / 2 };
	switch (_curStage)
	{
	case 1:
		// Stage2 장애물 생성
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset });
		break;
	case 2:
		// Stage3 장애물 생성
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset });

		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset });

		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset });

		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset });
		break;
	case 3:
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset });
			
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset });
		AddObject(ObjectType::Obstacle, { (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset });
		break;
	case 4:	// 게임 종료
		// Stage Reset
		_curStage = 1;
		
		// Player Pos Reset
		for (const auto& item : _players)
		{
			item.second->SetPos({ 400, 300 });
			g_framework->SendMovePacket(item.second, true);
		}

		// Room State Set
		_state = RoomState::Idle;
		break;
	}
}

void Room::ClearStage()
{
	// Player 제외 Stage에 있던 모든 Object 삭제
	for (const auto& itemItem : _items)
		g_framework->AddRemoveObject(itemItem.second);

	for (const auto& projectileItem : _projectiles)
		g_framework->AddRemoveObject(projectileItem.second);

	for (const auto& bombItem : _bombs)
		g_framework->AddRemoveObject(bombItem.second);

	for (const auto& obstacleItem : _obstacles)
		g_framework->AddRemoveObject(obstacleItem.second);
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
		return _bombs[id];
		break;
	case ObjectType::Obstacle:
		return _obstacles[id];
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