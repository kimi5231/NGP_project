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
	/*PlayerRef player = std::make_shared<Player>();
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
	_objects.push_back(bullet);*/
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	Scene::Update();

	ProcessInput();

	
}

void GameScene::Render(HDC hdc)
{
	HDC memDC, memDCImage;
	HBITMAP hbit, oldbit;

	// 더블 버퍼링을 위해 두 개의 메모리 DC 생성
	memDC = CreateCompatibleDC(hdc);
	memDCImage = CreateCompatibleDC(memDC);

	// hDC와 hbit 연결
	hbit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	// memDC hbit객체 선택
	oldbit = (HBITMAP)SelectObject(memDC, hbit);

	/*for (GameObject* object : _objects) {
		object->Render(memDC, memDCImage);
	}*//*
	for (PlayerRef player : _players) {
		player->Render(memDC, memDCImage);
	}

	_merchant->Render(memDC, memDCImage);

	_monster->Render(memDC, memDCImage);*/

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldbit);
	DeleteObject(hbit);
	DeleteDC(memDC);
	DeleteDC(memDCImage);
}

void GameScene::ProcessInput()
{
	// 코드 길어져서 포인터로 받기
	InputManager* input = GET_SINGLE(InputManager);
	// 연속 이동을 원하면 GetButton 사용 (키를 누르고 있는 동안 true)
	//for (GameObjectRef object : _objects) {
	//	if (object->GetObjectType() == ObjectType::Player) {
	//		if (input->GetButtonDown(KeyType::Left)) object->Left();
	//		if (input->GetButtonDown(KeyType::Right)) object->Right();
	//		if (input->GetButtonDown(KeyType::Up)) object->Up();
	//		if (input->GetButtonDown(KeyType::Down)) object->Down();
	//	}
	//}
	
	// 이동 버튼 GetButton으로 변경 시 주석 풀기
	/*if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->ResetCurFrame();
		}
	}*/
}