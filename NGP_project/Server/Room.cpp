#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "TimeManager.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"
#include "Projectile.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

Room::Room()
{
	InitializeCriticalSection(&_cs);

	_generateID = 1;
	_playerCount = 0;
	_state = RoomState::Idle;
	_timer = 50;
}

Room::~Room()
{
	DeleteCriticalSection(&_cs);
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

	EnterCriticalSection(&_cs);
	for (const auto& item : _objects)
	{
		item.second->Update();
	}
	LeaveCriticalSection(&_cs);

	//EnterCriticalSection(&_cs);
	for (const auto& object : _objects) {
		switch (object.second->GetObjectType()) {
		case ObjectType::Player:
			//object.second->Update();
			break;
		case ObjectType::ObstacleMonster:
		case ObjectType::NormalMonster:
		case ObjectType::RespawnMonster:
		case ObjectType::TankMonster:
		case ObjectType::BomberMonster:
		{
			Monster* monster = dynamic_cast<Monster*>(object.second.get());
			if (!monster) break;

			GameObject* closestPlayer = nullptr;
			float minDistance = std::numeric_limits<float>::infinity();

			// 가장 가까운 플레이어 탐색
			for (const auto& otherObject : _objects) {
				if (otherObject.second->GetObjectType() == ObjectType::Player) {
					float dx = otherObject.second->GetPos().x - monster->GetPos().x;
					float dy = otherObject.second->GetPos().y - monster->GetPos().y;
					float distance = sqrt(dx * dx + dy * dy);

					if (distance < minDistance) {
						minDistance = distance;
						closestPlayer = otherObject.second.get();
					}
				}
			}

			// 가장 가까운 플레이어에게만 이동
			if (closestPlayer) {
				monster->Update(closestPlayer);
			}
			break;
		}
		}
	}

	//// 상태가 Dead면 클라 연결 끊으면 오류남
	///*_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [](const GameObjectRef& o) {
	//	return o->IsState(ObjectState::Dead);
	//	}), _objects.end());*/
	//LeaveCriticalSection(&_cs);
}

GameObjectRef Room::AddObject(ObjectType type, Vertex pos, Dir dir)
{
	GameObjectRef object;

	switch (type)
	{
	case ObjectType::Player:
		object = std::make_shared<Player>();
		_playerCount++;
		break;
	case ObjectType::NormalMonster:
		object = std::make_shared<NormalMonster>();
		break;
	case ObjectType::TankMonster:
		object = std::make_shared<TankMonster>();
		break;
	case ObjectType::BomberMonster:
		object = std::make_shared<BomberMonster>();
		break;
	case ObjectType::RespawnMonster:
		object = std::make_shared<RespawnMonster>();
		break;
	case ObjectType::ObstacleMonster:
		object = std::make_shared<ObstacleMonster>();
		break;
	case ObjectType::Item:
		break;
	case ObjectType::Bullet:
		object = std::make_shared<Projectile>(dir, pos);
		break;
	case ObjectType::Bomb:
		break;
	case ObjectType::UI:
		break;
	case ObjectType::Obstacle:
		break;
	}

	object->SetID(_generateID++);

	EnterCriticalSection(&_cs);
	_objects[object->GetID()] = object;
	LeaveCriticalSection(&_cs);

	g_framework->SendAddObjectPacket(object, true);

	return object;
}

void Room::RemoveObject(int id)
{
	GameObjectRef object = _objects[id];

	EnterCriticalSection(&_cs);
	_objects.erase(id);
	LeaveCriticalSection(&_cs);

	g_framework->SendRemoveObjectPacket(object, true);
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
	
	//// item, bomb 생성을 위한 콜백함수 설정
	//monster->SetCallback([this](GameObject* obj) {
	//	this->AddObject(obj);
	//	});
}