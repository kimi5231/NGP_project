#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

GameScene::GameScene(HINSTANCE hInst)
{
	_players.push_back(std::make_shared<Player>(hInst));
	//_objects.push_back(new GameObject(hInst));

	_monster = new TankMonster(hInst);
}

GameScene::~GameScene()
{
	for (GameObject* object : _objects) {
		delete object;
	}
}

void GameScene::Init()
{
}

void GameScene::Update()
{
}

void GameScene::Render()
{
}

void GameScene::ProcessInput(InputManager* inputManager)
{
	// 연속 이동을 원하면 GetButton 사용 (키를 누르고 있는 동안 true)
	if (inputManager->GetButton(KeyType::Left)) {
		for (PlayerRef player : _players) {
			player->Left();
		}
	}
	if (inputManager->GetButton(KeyType::Right)) {
		for (PlayerRef player : _players) {
			player->Right();
		}
	}
	if (inputManager->GetButton(KeyType::Up)) {
		for (PlayerRef player : _players) {
			player->Up();
		}
	}
	if (inputManager->GetButton(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->Down();
		}
	}

}

void GameScene::Render(HDC hDC)
{
	HDC memDC, memDCImage;
	HBITMAP hbit, oldbit;

	// 더블 버퍼링을 위해 두 개의 메모리 DC 생성
	memDC = CreateCompatibleDC(hDC);
	memDCImage = CreateCompatibleDC(memDC);

	// hDC와 hbit 연결
	hbit = CreateCompatibleBitmap(hDC, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	// memDC hbit객체 선택
	oldbit = (HBITMAP)SelectObject(memDC, hbit);

	/*for (GameObject* object : _objects) {
		object->Render(memDC, memDCImage);
	}*/
	for (PlayerRef player : _players) {
		player->Render(memDC, memDCImage);
	}

	//_monster->Render(memDC, memDCImage);

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hDC, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldbit);
	DeleteObject(hbit);
	DeleteDC(memDC);
	DeleteDC(memDCImage);
}
