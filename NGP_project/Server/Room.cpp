#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "TimeManager.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"
#include "Projectile.h"
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
	/*static float sumTime;
	sumTime += GET_SINGLE(TimeManager)->GetDeltaTime();
	if (sumTime > 1)
	{
		sumTime = 0;
		_timer -= 1;
		g_framework->SendUpdateTimerPacket(true);
	}*/
	
	SpawnMonster();

	/*EnterCriticalSection(&_cs);
	for (const auto& item : _objects)
	{
		item.second->Update();
	}
	LeaveCriticalSection(&_cs);*/

	EnterCriticalSection(&_cs);
	for (const auto& object : _objects) {
		object.second->Update();
	}
	LeaveCriticalSection(&_cs);

	EnterCriticalSection(&_cs);
	std::erase_if(_objects, [](const auto& kv) {
		return kv.second->IsState(ObjectState::Dead);
		});
	LeaveCriticalSection(&_cs);
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
		// item, bomb 생성을 위한 콜백함수 설정
		dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::TankMonster:
		object = std::make_shared<TankMonster>();
		dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::BomberMonster:
		object = std::make_shared<BomberMonster>();
		dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::RespawnMonster:
		object = std::make_shared<RespawnMonster>();
		dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::ObstacleMonster:
		object = std::make_shared<ObstacleMonster>();
		dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
			this->AddObject(obj->GetObjectType(), obj->GetPos(), obj->GetDir());
			});
		break;
	case ObjectType::Item:
		break;
	case ObjectType::Bullet:
		object = std::make_shared<Projectile>(dir, pos);
		break;
	case ObjectType::Bomb:
		object = std::make_shared<BombObject>(pos);
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
}