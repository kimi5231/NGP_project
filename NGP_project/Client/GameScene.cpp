#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Global.h"
#include "Item.h"
#include "projectile.h"

// Monster
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

GameScene::GameScene()
{
	PlayerRef player = std::make_shared<Player>();
	player->SetObjectType(ObjectType::Player);
	_objects.push_back(player);

	TankMonsterRef TM = std::make_shared<TankMonster>();
	TM->SetObjectType(ObjectType::Monster);
	_objects.push_back(TM);

	ItemRef item = std::make_shared<Item>(ItemType::Life);
	item->SetObjectType(ObjectType::Item);
	item->SetPos({ 50, 50 });
	_objects.push_back(item);

	ProjectileRef bullet = std::make_shared<Projectile>(Dir::RightDown);
	bullet->SetState(ObjectState::Move);
	_objects.push_back(bullet);
}

GameScene::~GameScene()
{
	_objects.clear();
}

void GameScene::Update()
{
	Scene::Update();

	ProcessInput();

	// test 용
	for (GameObjectRef object : _objects)
	{
		if (object->GetObjectType() == ObjectType::Monster)
			object->Right();
	}
}

void GameScene::Render(HDC hdc)
{
	Scene::Render(hdc);
}

void GameScene::ProcessInput()
{
	// 코드 길어져서 포인터로 받기
	InputManager* input = GET_SINGLE(InputManager);
	// 연속 이동을 원하면 GetButton 사용 (키를 누르고 있는 동안 true)
	if (input->GetButtonDown(KeyType::Left)) 
	{
		for (GameObjectRef object : _objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
				object->Left();
		}
	} 
	
	if (input->GetButtonDown(KeyType::Right)) 
	{
		for (GameObjectRef object : _objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
				object->Right();
		}
	} 
	
	if (input->GetButtonDown(KeyType::Up)) 
	{
		for (GameObjectRef object : _objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
				object->Up();
		}
	} 
	
	if (input->GetButtonDown(KeyType::Down)) 
	{
		for (GameObjectRef object : _objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
				object->Down();
		}
	}

	// 이동 버튼 GetButton으로 변경 시 주석 풀기
	/*if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->ResetCurFrame();
		}
	}*/
}