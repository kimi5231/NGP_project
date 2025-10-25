#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Merchant.h"
#include "Global.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

GameScene::GameScene()
{
	//_players.push_back(std::make_shared<Player>());
	Player* player = new Player();
	_objects.push_back(player);

	TankMonster* TM = new TankMonster();
	_objects.push_back(TM);
	_monster = TM;
}

GameScene::~GameScene()
{
	for (GameObject* object : _objects)
		delete object;

	_objects.clear();
}

void GameScene::Update()
{
	ProcessInput();
	_monster->Right();	// test 용
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
	/*if (input->GetButtonDown(KeyType::Left)) {
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
	}*/

	// 이동 버튼 GetButton으로 변경 시 주석 풀기
	/*if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Right) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down)) {
		for (PlayerRef player : _players) {
			player->ResetCurFrame();
		}
	}*/
}