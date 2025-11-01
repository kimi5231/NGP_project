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

#define BOARD_SIZE 16
HBITMAP hCellBitmap;

GameScene::GameScene()
{
	_players.push_back(new Player);
	_monsters.push_back(new TankMonster);
	hCellBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "sand_background.bmp").wstring().c_str() , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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
	for (Monster* monster : _monsters) {
		monster->Update(_players[0]);
	}

	//GET_SINGLE(InputManager)->Update();
	ProcessInput();

	if (_players[0]->IsCollision(_monsters[0])) {
		//_players[0]->Left();	// test 용
	}
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

	// 배경
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			int cellX = col * CELL_SIZE;
			int cellY = row * CELL_SIZE;
			// 한 칸의 비트맵 그리기
			oldbit = (HBITMAP)SelectObject(memDCImage, hCellBitmap);
			BitBlt(memDC, cellX, cellY, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCCOPY);
		}
	}
	
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