#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "TimeManager.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"

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

	//// Timer Update
	//static float sumTime;
	//sumTime += GET_SINGLE(TimeManager)->GetDeltaTime();
	//if (sumTime > 1)
	//{
	//	sumTime = 0;
	//	_timer -= 1;
	//	g_framework->SendUpdateTimerPacket();
	//}
	//
	SpawnMonster();

	for (const auto& item : _objects)
	{
		//item.second->Update();
	}

	EnterCriticalSection(&_cs);
	//for (const auto& object : _objects) {
	//	switch (object.second->GetObjectType()) {
	//	case ObjectType::Player:
	//		object.second->Update();
	//		break;
	//	case ObjectType::ObstacleMonster:
	//	case ObjectType::NormalMonster:
	//	case ObjectType::RespawnMonster:
	//	case ObjectType::TankMonster:
	//	case ObjectType::BomberMonster:
	//		// bomb object 생성 callBack 함수
	//		/*dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
	//			this->AddObject(obj);
	//			});*/
	//		for (const auto& otherObject : _objects) {
	//			switch (otherObject.second->GetObjectType()) {
	//			case ObjectType::Player:	// player 객체를 몬스터 update에 넘겨줌
	//				dynamic_cast<Monster*>(object.second.get())->Update(otherObject.second.get());
	//				break;
	//			case ObjectType::Bullet:	// 충알 충돌처리
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//		break;
	//	}
	//}

	//// 상태가 Dead면 클라 연결 끊으면 오류남
	///*_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [](const GameObjectRef& o) {
	//	return o->IsState(ObjectState::Dead);
	//	}), _objects.end());*/
	LeaveCriticalSection(&_cs);
}

GameObjectRef Room::AddObject(ObjectType type)
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

	g_framework->SendAddObjectPacket(object);

	return object;
}

void Room::RemoveObject(int id)
{
	GameObjectRef object = _objects[id];

	EnterCriticalSection(&_cs);
	_objects.erase(id);
	LeaveCriticalSection(&_cs);

	g_framework->SendRemoveObjectPacket(object);
}


void Room::SpawnMonster()
{
	// 시간에 따라 몬스터 추가
	static float monsterSpawnTimer{};

	if (GET_SINGLE(TimeManager)->CheckTimer(monsterSpawnTimer, MONSTER_SPAWN_TIME))
	{
		g_framework->SendUpdateTimerPacket();
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