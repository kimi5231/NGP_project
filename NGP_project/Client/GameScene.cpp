#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Merchant.h"

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

	_merchant = new Merchant(hInst);
}

GameScene::~GameScene()
{
	for (GameObject* object : _objects) {
		delete object;
	}
	delete _merchant;
	delete _monster;
}

void GameScene::Init()
{
}

void GameScene::Update()
{
	ProcessInput();
	_monster->Right();	// test 용
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

	_merchant->Render(memDC, memDCImage);

	_monster->Render(memDC, memDCImage);

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hDC, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

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
	if (input->GetButtonDown(KeyType::Left)) {
		for (PlayerRef player : _players) {
			player->Left();
		}
	} if (input->GetButtonDown(KeyType::Right)) {
		for (PlayerRef player : _players) {
			player->Right();
		}
	} if (input->GetButtonDown(KeyType::Up)) {
		for (PlayerRef player : _players) {
			player->Up();
		}
	} if (input->GetButtonDown(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->Down();
		}
	}

	// 이동 버튼 GetButton으로 변경 시 주석 풀기
	/*if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->ResetCurFrame();
		}
	}*/
}