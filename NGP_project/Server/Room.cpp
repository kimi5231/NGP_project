#include "pch.h"
#include "Room.h"
#include "Monster.h"
#include "Player.h"

Room::Room()
{
	InitializeCriticalSection(&_cs);

	_generateID = 1;
}

Room::~Room()
{
	DeleteCriticalSection(&_cs);
}

void Room::Update()
{
	EnterCriticalSection(&_cs);
	for (const auto& object : _objects) {
		switch (object->GetObjectType()) {
		case ObjectType::Player:
			object->Update();
			break;
		case ObjectType::Monster:
			// bomb object 생성 callBack 함수
			/*dynamic_cast<Monster*>(object.get())->SetCallback([this](GameObject* obj) {
				this->AddObject(obj);
				});*/
			for (const auto& otherObject : _objects) {
				switch (object->GetObjectType()) {
				case ObjectType::Player:	// player 객체를 몬스터 update에 넘겨줌
					dynamic_cast<Monster*>(object.get())->Update(otherObject.get());
					break;
				case ObjectType::Bullet:	// 충알 충돌처리
					/*if (object->IsCollision(otherObject.get()) && !object->IsState(ObjectState::Dead) && !object->IsState(ObjectState::Revive)) {
						object->Damaged(dynamic_cast<Projectile*>(otherObject.get())->GetDamage());
						otherObject->SetState(ObjectState::Dead);
					}*/
					break;
				default:
					break;
				}
			}
			object->Update();
			break;
		}
	}

	// 상태가 Dead면 삭제
	/*_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [](const GameObjectRef& o) {
		return o->IsState(ObjectState::Dead);
		}), _objects.end());*/
	LeaveCriticalSection(&_cs);
}

GameObjectRef Room::AddObject(ObjectType type)
{
	if (type == ObjectType::Player)
	{
		// Player 생성
		PlayerRef player = std::make_shared<Player>();
		player->SetID(_generateID++);
		EnterCriticalSection(&_cs);
		_objects.push_back(player);
		LeaveCriticalSection(&_cs);
		return player;
	}

	// 추후 다른 객체 클래스 추가되면 코드 추가 예정
}