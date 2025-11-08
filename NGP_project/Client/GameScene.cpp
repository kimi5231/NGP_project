#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Global.h"
#include "Item.h"
#include "projectile.h"
#include "BombObject.h"
#include "BoundingBox.h"
#include "Button.h"

// Monster
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"

#define BULLET_TIMER 30

HBITMAP gBackgroundBitmap;
RECT gBackgoundRect{ 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };	// 이 수치를 조정해서 배경화면 그리기

GameScene::GameScene()
{
	_players.push_back(std::make_shared <Player>());
	_monsters.push_back(std::make_shared <BomberMonster>());
	_monsters.push_back(std::make_shared <TankMonster>());
	_monsters.push_back(std::make_shared <ObstacleMonster>());
	_monsters.push_back(std::make_shared <RespawnMonster>());
	_objects.push_back(std::make_shared<Button>(Vertex{ 400, 300 }, Vertex{100, 100}));

	gBackgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "sand_background.bmp").wstring().c_str() , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

GameScene::~GameScene()
{
}

void GameScene::AddObject(GameObject* obj)
{
	_objects.push_back(std::shared_ptr<GameObject>(obj));
}

void GameScene::Update()
{
	for (const auto& object : _objects) {
		object->Update();

	}
	for (const auto& monster : _monsters) {
		monster->Update(_players[0].get());
		monster->SetCallback([this](GameObject* obj) {
			this->AddObject(obj);
			});
		for (const auto& object : _objects) {
			if (object->GetObjectType() == ObjectType::Bullet) {
				monster->Damaged(dynamic_cast<Projectile*>(object.get())->GetDamage());
			}
		}
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
		player->GetBoundingBox().Render(memDC, memDCImage);	// 디버깅용
	}

	//_merchant->Render(memDC, memDCImage);

	for (const auto& monster : _monsters) {
		monster->Render(memDC, memDCImage);
		monster->GetBoundingBox().Render(memDC, memDCImage);	// 디버깅용
	}

	for (const auto object : _objects) {
		object->Render(memDC, memDCImage);
		object->GetBoundingBox().Render(memDC, memDCImage);	// 디버깅용
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

	static int cnt{};
	cnt++;
	for (const auto& player : _players) {
		// 이동
		if (input->GetButton(KeyType::A))  player->Left();
		if (input->GetButton(KeyType::D)) player->Right();
		if (input->GetButton(KeyType::W))    player->Up();
		if (input->GetButton(KeyType::S))  player->Down();

		// 총알 발사
		if (cnt % BULLET_TIMER == 0) {
			Vertex playerPos = player->GetPos();
			if (input->GetButton(KeyType::Up)) {
				if (input->GetButton(KeyType::Right)) _objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos));
				else if (input->GetButton(KeyType::Left)) _objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
				else _objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
			}
			else if (input->GetButton(KeyType::Down)) {
				if (input->GetButton(KeyType::Right)) _objects.push_back(std::make_shared<Projectile>(Dir::RightDown, playerPos));
				else if (input->GetButton(KeyType::Left)) _objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
				else _objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
			}
			else if (input->GetButton(KeyType::Right)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
			}
			else if (input->GetButton(KeyType::Left)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
			}
		}

		// 키 Up
		if (input->GetButtonUp(KeyType::W) || input->GetButtonUp(KeyType::A) || input->GetButtonUp(KeyType::S) || input->GetButtonUp(KeyType::D)) {
			player->ResetCurFrame();
		}
	}

	if (input->GetButtonDown(KeyType::LeftMouse)) {
		for (const auto object : _objects) {
			if (object->GetObjectType() == ObjectType::Button) {
				if (object->IsClick(input->GetMousePos())) {
					_monsters.push_back(std::make_shared<TankMonster>());	// test
				}
			}
		}
	}

}