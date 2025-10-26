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
	_players.push_back(new Player);
	_monsters.push_back(new TankMonster);
}

GameScene::~GameScene()
{
	if (_players.data()) {
		for (Player* player : _players) {
			delete player;
		}
	}
	if (_monsters.data()) {
		for (Monster* monster: _monsters) {
			delete monster;
		}
	}
}

void GameScene::Update()
{
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

	
	for (Player* player : _players) {
		player->Render(memDC, memDCImage);
	}

	//_merchant->Render(memDC, memDCImage);

	for (Monster* monster : _monsters) {
		monster->Render(memDC, memDCImage);
	}

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
	for (Player* player : _players) {
		if (input->GetButtonDown(KeyType::Left)) player->Left();
		if (input->GetButtonDown(KeyType::Right)) player->Right();
		if (input->GetButtonDown(KeyType::Up)) player->Up();
		if (input->GetButtonDown(KeyType::Down)) player->Down();
	}
	
	// 이동 버튼 GetButton으로 변경 시 주석 풀기
	/*if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->ResetCurFrame();
		}
	}*/
}