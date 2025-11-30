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
	
	//SpawnMonster();

	EnterCriticalSection(&_cs);
	for (const auto& item : _objects)
	{
		item.second->Update();
	}
	LeaveCriticalSection(&_cs);

	//EnterCriticalSection(&_cs);
	//for (const auto& object : _objects) {
	//	switch (object->GetObjectType()) {
	//	case ObjectType::Player:
	//		object->Update();
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
	//			switch (object->GetObjectType()) {
	//			case ObjectType::Player:	// player 객체를 몬스터 update에 넘겨줌
	//				dynamic_cast<Monster*>(object.get())->Update(otherObject.get());
	//				break;
	//			case ObjectType::Bullet:	// 충알 충돌처리
	//				break;
	//			default:
	//				break;
	//			}
	//		}

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

void Room::InitObstalce()
{
	// 클라이언트에서 해도 되는 작업

	// 이미 배열이 있으면 해제 후 재할당

	//int sizeOffset{ CELL_SIZE / 2 };	// 위치를 맞추기 위한 값
	//// 가로
	//for (int i = 0; i < BOARD_SIZE; ++i) {
	//	if (i >= 7 && i <= 9) continue;
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (gBackgroundRect.left + sizeOffset) + i * CELL_SIZE, gBackgroundRect.top + sizeOffset }));
	//}
	//for (int i = 0; i < BOARD_SIZE; ++i) {
	//	if (i >= 7 && i <= 9) continue;
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset + i * CELL_SIZE, gBackgroundRect.top + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE }));
	//}
	//// 세로
	//for (int i = 1; i < BOARD_SIZE - 1; ++i) {
	//	if (i >= 7 && i <= 9) continue;
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset, gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
	//}
	//for (int i = 1; i < BOARD_SIZE - 1; ++i) {
	//	if (i >= 7 && i <= 9) continue;
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE, gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
	//}

	//switch (_curStage) {
	//case 1:
	//	break;
	//case 2:
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
	//	break;
	//case 3:
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset }));

	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset }));

	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));

	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
	//	break;
	//case 4:
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));

	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
	//	_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
	//	break;
	//}
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