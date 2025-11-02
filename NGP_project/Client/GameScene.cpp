#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Global.h"
#include "Item.h"
#include "projectile.h"
#include "BombObject.h"

// Monster
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"


HBITMAP gBackgroundBitmap;
RECT gBackgoundRect{ 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };	// 이 수치를 조정해서 배경화면 그리기


GameScene::GameScene()
{
	_players.push_back(std::make_shared <Player>());
	_monsters.push_back(std::make_shared < TankMonster>());
	_objects.push_back(std::make_shared<BombObject>());

	gBackgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "sand_background.bmp").wstring().c_str() , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	for (const auto& monster : _monsters) {
		monster->Update(_players[0].get());
	}
	for (const auto object : _objects) {
		object->Update();
	}
	_objects.erase(std::remove_if(_objects.begin(), _objects.end(),[](const GameObjectRef& o) {
			return o->IsDead();
		}),	_objects.end());


	ProcessInput();

	if (_players[0]->IsCollision(_monsters[0].get())) {
		//_players[0]->Left();	// test 용
	}
}

void GameScene::Render(HDC hdc)
{
	HDC memDC, memDCImage;
	HBITMAP hbit, oldbit[2];

	// 더블 버퍼링을 위해 두 개의 메모리 DC 생성
	memDC = CreateCompatibleDC(hdc);
	memDCImage = CreateCompatibleDC(memDC);

	// hDC와 hbit 연결
	hbit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	// memDC hbit객체 선택
	oldbit[0] = (HBITMAP)SelectObject(memDC, hbit);

	// 배경
	BITMAP bmpInfo;
	GetObject(gBackgroundBitmap, sizeof(BITMAP), &bmpInfo);
	oldbit[1] = (HBITMAP)SelectObject(memDCImage, gBackgroundBitmap);
	StretchBlt(memDC, gBackgoundRect.left, gBackgoundRect.top, gBackgoundRect.right, gBackgoundRect.bottom, memDCImage, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

	// GameObject
	for (const auto& player : _players) {
		player->Render(memDC, memDCImage);
	}

	//_merchant->Render(memDC, memDCImage);

	for (const auto& monster : _monsters) {
		monster->Render(memDC, memDCImage);
	}

	for (const auto object : _objects) {
		object->Render(memDC, memDCImage);
	}

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldbit[0]);
	SelectObject(memDCImage, oldbit[1]);
	DeleteObject(hbit);
	DeleteDC(memDC);
	DeleteDC(memDCImage);
}

void GameScene::ProcessInput()
{
	// 코드 길어져서 포인터로 받기
	InputManager* input = GET_SINGLE(InputManager);
	// 연속 이동을 원하면 GetButton 사용 (키를 누르고 있는 동안 true)
	for (const auto& player : _players) {
		if (input->GetButton(KeyType::Left)) player->Left();
		if (input->GetButton(KeyType::Right)) player->Right();
		if (input->GetButton(KeyType::Up)) player->Up();
		if (input->GetButton(KeyType::Down)) player->Down();
	}

	for (const auto& player : _players) {
		Vertex playerPos = player->GetPos();
		if (input->GetButton(KeyType::W)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
		}
		if (input->GetButton(KeyType::D)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
		}
		if (input->GetButton(KeyType::A)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
		}
		if (input->GetButton(KeyType::S)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
		}
	}
	
	if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (const auto& player : _players) {
			player->ResetCurFrame();
		}
	}


}