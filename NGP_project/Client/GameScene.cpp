#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "Global.h"
#include "Item.h"
#include "projectile.h"
#include "BombObject.h"

// Monster
#include "Monster.h"
#include "BomberMonster.h"
#include "NormalMonster.h"
#include "ObstacleMonster.h"
#include "RespawnMonster.h"
#include "TankMonster.h"


HBITMAP gBackgroundBitmap;
float bulletSpeed{ BULLET_TIME };
// 아이템 사용 관련 전역 변수
bool useLightning{}, useWaterWheel{}, useShotgun{}, useHourglass{};

void GameScene::InitObstalce()
{
	// 이미 배열이 있으면 해제 후 재할당

	int sizeOffset{ CELL_SIZE / 2 };
	switch (_curStage) {
	case 1:
		// 가로
		for (int i = 0; i < BOARD_SIZE; ++i) {
			if (i >= 7 && i <= 9) continue;
			_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (gBackgroundRect.left + sizeOffset) + i * CELL_SIZE, gBackgroundRect.top + sizeOffset }));
		}
		for (int i = 0; i < BOARD_SIZE; ++i) {
			if (i >= 7 && i <= 9) continue;
			_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset + i * CELL_SIZE, gBackgroundRect.top + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE }));
		}
		// 세로
		for (int i = 1; i < BOARD_SIZE - 1; ++i) {
			if (i >= 7 && i <= 9) continue;
			_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset, gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
		}
		for (int i = 1; i < BOARD_SIZE - 1; ++i) {
			if (i >= 7 && i <= 9) continue;
			_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ gBackgroundRect.left + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE, gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
		}
		break;
	}
}
GameScene::GameScene()
{
	_monsters.push_back(std::make_shared <BomberMonster>());
	_monsters.push_back(std::make_shared <TankMonster>());
	_monsters.push_back(std::make_shared <ObstacleMonster>());
	_monsters.push_back(std::make_shared <RespawnMonster>());
	_objects.push_back(std::make_shared<Item>(ItemType::Life, Vertex{ 400, 100 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Magazine, Vertex{ 400, 200 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Lightning, Vertex{ 400, 300 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Waterwheel, Vertex{ 400, 500 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Coffee, Vertex{ 300, 500 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Shotgun, Vertex{ 500, 500 }));
	_objects.push_back(std::make_shared<Item>(ItemType::Hourglass, Vertex{ 400, 600 }));
	//_objects.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ 400, 400 }));
	InitObstalce();
	
	// Sound        
	//GET_SINGLE(SoundManager)->Play(L"main_music", true);

	// UI
	_ui.push_back(std::make_shared<Button>(Vertex{ 50, 400 }, Vertex{100, 100}, L"button"));
	_ui.push_back(std::make_shared<UI>(Vertex{ 70, 100 }, Vertex{100, 100}, L"Item UI"));
	DWORD uiColor{ RGB(50, 50, 50) };
	_ui.push_back(std::make_shared<UI>(Vertex{ 70, 200 }, Vertex{100, 50}, L"Life: ", uiColor, true));

	gBackgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "sand_background.bmp").wstring().c_str() , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	if (_players.empty())
		return;

	for (const auto& object : _objects) {
		object->Update();
		// 아이템 충돌 처리
		if (object->GetObjectType() == ObjectType::Item && _players[0]->IsCollision(object.get())) {
			_players[0]->SetItem(dynamic_pointer_cast<Item>(object));
			object->SetState(ObjectState::Dead);
		}

		// 장애물
		if (object->GetObjectType() == ObjectType::Obstacle && _players[0]->IsCollision(object.get())) {
			_players[0]->UndoPos();
		}
	}
	for (const auto& monster : _monsters) {
		monster->Update(_players[0].get());

		monster->SetCallback([this](GameObject* obj) {
			this->AddObject(obj);
			});
		// 몬스터-총알 충돌 처리
		for (const auto& object : _objects) {
			if (object->GetObjectType() == ObjectType::Bullet) {
				if (monster->IsCollision(object.get()) && !monster->IsState(ObjectState::Dead) && monster->CanDamage()) {
					monster->Damaged(dynamic_cast<Projectile*>(object.get())->GetDamage());
					object->SetState(ObjectState::Dead);
				}
			}
			// 장애물
			if (object->GetObjectType() == ObjectType::Obstacle && monster->IsCollision(object.get())) {
				monster->UndoPos();
			}
		}

		// 몬스터끼리 충돌 처리
		for (const auto& otherMonster : _monsters) {
			if (otherMonster == monster) continue;
			if (monster->IsCollision(otherMonster.get())) {
				if(monster->GetPos() != monster->GetPrevPos())
				monster->UndoPos();
			}
		}
		// 시계 아이템 사용 시
		if (useHourglass)
			monster->_status._speed = 0;
		else
			monster->_status._speed = MONSTER_SPEED;
	}
	_players[0]->Update();

	if (useLightning) {	// 번개 아이템 사용 시
		_monsters.clear();
		useLightning = false;
	}

	_monsters.erase(std::remove_if(_monsters.begin(), _monsters.end(), [](const MonsterRef& o) {
		return o->IsState(ObjectState::Dead);
		}), _monsters.end());

	_objects.erase(std::remove_if(_objects.begin(), _objects.end(),[](const GameObjectRef& o) {
			return o->IsState(ObjectState::Dead);
		}),	_objects.end());

	_timerUI.Update(_stagetime);

	ProcessInput();
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
	StretchBlt(memDC, gBackgroundRect.left, gBackgroundRect.top, gBackgroundRect.right- gBackgroundRect.left, gBackgroundRect.bottom - gBackgroundRect.top, memDCImage, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

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

	// UI
	for (const auto ui : _ui) {
		//ui->Render(memDC, memDCImage, _players[0]->_status._life);	// 나중에 수정
	}
	_timerUI.Render(memDC, memDCImage, _stagetime);

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldbit[0]);
	SelectObject(memDCImage, oldbit[1]);
	DeleteObject(hbit);
	DeleteDC(memDC);
	DeleteDC(memDCImage);
}

void GameScene::AddPlayer(Player* player)
{
	std::shared_ptr<Player> p(player);
	_players.push_back(p);

	// MyPlayer 설정
	if (!_localPlayer)
		_localPlayer = p;
}

void GameScene::AddMonster(Monster* monster)
{
	_monsters.push_back(std::shared_ptr<Monster>(monster));
}

void GameScene::AddObject(GameObject* object)
{	
	_objects.push_back(std::shared_ptr<GameObject>(object));
}

void GameScene::ProcessInput()
{
	if (!_localPlayer)
		return;

	// 코드 길어져서 포인터로 받기
	InputManager* input = GET_SINGLE(InputManager);

	static bool prevKeyUp{};
	// 키 입력은 첫 번째 플레이어(자기자신)만 받음
	// 이동
	Vertex direction{};
	if (input->GetButton(KeyType::A)) direction.x -= 1;
	if (input->GetButton(KeyType::D)) direction.x += 1;
	if (input->GetButton(KeyType::W)) direction.y -= 1;
	if (input->GetButton(KeyType::S)) direction.y += 1;

	if (direction.x != 0 || direction.y != 0) {
		_localPlayer->Move(direction);
	}

	// 총알 발사
	if (prevKeyUp || CheckTimer(_localPlayer->_timer, bulletSpeed)) {
		Vertex playerPos = _localPlayer->GetPos();
		if (input->GetButton(KeyType::Up)) {
			if (input->GetButton(KeyType::Right)) { 
				_objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos)); 
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
				}
			}
			else if (input->GetButton(KeyType::Left)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
				}
			}
			else {
				_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
				}
			}
			prevKeyUp = false;
		}
		else if (input->GetButton(KeyType::Down)) {
			if (input->GetButton(KeyType::Right)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::RightDown, playerPos));
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
				}
			}
			else if (input->GetButton(KeyType::Left)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
				}
			}
			else {
				_objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
				if (useShotgun) {
					_objects.push_back(std::make_shared<Projectile>(Dir::RightDown, playerPos));
					_objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
				}
			}
			prevKeyUp = false;
		}
		else if (input->GetButton(KeyType::Right)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
			if (useShotgun) {
				_objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::RightDown, playerPos));
			}
			prevKeyUp = false;
		}
		else if (input->GetButton(KeyType::Left)) {
			_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
			if (useShotgun) {
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
			}
			prevKeyUp = false;
		}

		// 물래방아 아이템 8방향으로 발사
		if (useWaterWheel) {
			if (input->GetButton(KeyType::Right) || input->GetButton(KeyType::Up) || input->GetButton(KeyType::Down) || input->GetButton(KeyType::Left)) {
				_objects.push_back(std::make_shared<Projectile>(Dir::Down, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::Right, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::RightDown, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
				_objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
			}
		}
	}

	// 이동 키 Up
	if (input->GetButtonUp(KeyType::W) || input->GetButtonUp(KeyType::A) || input->GetButtonUp(KeyType::S) || input->GetButtonUp(KeyType::D)) {
		_localPlayer->ResetCurFrame();
		_localPlayer->SetState(ObjectState::Idle);
	}
	if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down) || input->GetButtonUp(KeyType::Right)) {
		prevKeyUp = true;
		_localPlayer->_timer = 0.0f;
	}

	if (input->GetButtonDown(KeyType::LeftMouse)) {
		for (const auto button : _ui) {
			if (button->GetObjectType() == ObjectType::Button && button->Intersects(input->GetMousePos())) {
				_monsters.push_back(std::make_shared<TankMonster>());	// test
			}
		}
	}

	// 아이템 사용
	if (input->GetButtonDown(KeyType::SpaceBar)) {
		_localPlayer->UseItem();
	}

}


