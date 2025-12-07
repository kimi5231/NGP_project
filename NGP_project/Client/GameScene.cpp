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

#include "GameNetwork.h"

HBITMAP gBackgroundBitmap;
float bulletSpeed{ BULLET_TIME };
// 아이템 사용 관련 전역 변수
bool useLightning{}, useWaterWheel{}, useShotgun{}, useHourglass{};

GameScene::GameScene()
{
	// Sound        
	//GET_SINGLE(SoundManager)->Play(L"main_music", true);

	// UI
	_ui.push_back(std::make_shared<Button>(Vertex{ 50, 400 }, Vertex{100, 100}, L"button"));
	_ui.push_back(std::make_shared<UI>(Vertex{ 70, 100 }, Vertex{100, 100}, L"Item UI"));
	DWORD uiColor{ RGB(50, 50, 50) };
	_ui.push_back(std::make_shared<UI>(Vertex{ 70, 200 }, Vertex{100, 50}, L"Life: ", uiColor, true));

	gBackgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "sand_background.bmp").wstring().c_str() , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// Obstacle
	CreateObstacle();
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	EnterCriticalSection(&g_cs);
	if (!_localPlayer.get())
	{
		LeaveCriticalSection(&g_cs);
		return;
	}

	if (_localPlayer->GetState() != ObjectState::Dead)
		ProcessInput();
	LeaveCriticalSection(&g_cs);

	EnterCriticalSection(&g_cs);
	for (const auto& object : _objects) {
		object.second->Update();
	}
	LeaveCriticalSection(&g_cs);

	EnterCriticalSection(&g_cs);
	_localPlayer->Update();
	LeaveCriticalSection(&g_cs);

	EnterCriticalSection(&g_cs);
	if ( _isStayButtonActive && _stayButton && _leaveButton)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (_stayButton->Intersects(mousePos))
			{
				_isStayButtonActive = false;
			}
			if (_leaveButton->Intersects(mousePos))
			{
				PostQuitMessage(0);
			}
		}
	}
	LeaveCriticalSection(&g_cs);
	//		// 장애물
	//		if (type == ObjectType::Obstacle && monster->IsCollision(object.get())) {
	//			ObjectType monsterType = monster->GetObjectType();
	//			// 폭탄/장애물 몬스터는 장애물에 걸리면 경로 다시 탐색
	//			if (monsterType == ObjectType::BomberMonster || monsterType == ObjectType::ObstacleMonster) {
	//				monster->FindTarget(nullptr);
	//			}
	//			monster->UndoPos();
	//		}
	//	}
	//	// 시계 아이템 사용 시
	//	if (useHourglass)
	//		monster->_status._speed = 0;
	//	else
	//		monster->_status._speed = MONSTER_SPEED;

	//if (useLightning) {	// 번개 아이템 사용 시
	//	_monsters.clear();
	//	useLightning = false;
	//}
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

	EnterCriticalSection(&g_cs);

	// GameObject
	// Obstacle 
	for (const auto obstacle : _obstacles)
	{
		obstacle->Render(memDC, memDCImage);
	}

	// Local Player
	if (_localPlayer) {
		if (_localPlayer->GetState() != ObjectState::Dead)
		{
			_localPlayer->Render(memDC, memDCImage);
			_localPlayer->GetBoundingBox().Render(memDC, memDCImage, RGB(255, 0, 0));
		}
	}

	// Other Player
	for (const auto& [id, player] : _players) {
		if (player->GetState() != ObjectState::Dead)
		{
			player->Render(memDC, memDCImage);
			player->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 255, 0));	// 디버깅용
		}
	}

	// Monster
	for (const auto& [id, monster] : _monsters) {
		monster->Render(memDC, memDCImage);
		monster->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 0, 0));	// 디버깅용
	}

	// Objects
	for (const auto [id, object] : _objects) {
		object->Render(memDC, memDCImage);
		object->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 0, 0));	// 디버깅용
	}

	// UI
	if (_localPlayer.get()) {
		for (const auto ui : _ui) {
			ui->Render(memDC, memDCImage, _localPlayer->_status._life);	// 나중에 수정
		}
	}
	if ( _isStayButtonActive && _stayButton && _leaveButton)
	{
		_stayButton->Render(memDC, memDCImage, 0);
		_leaveButton->Render(memDC, memDCImage, 0);
	}
	_timerUI.Render(memDC);
	
	LeaveCriticalSection(&g_cs);

	// hDC에 memDC 출력(최종화면 출력)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldbit[0]);
	SelectObject(memDCImage, oldbit[1]);
	DeleteObject(hbit);
	DeleteDC(memDC);
	DeleteDC(memDCImage);
}

void GameScene::CreateObstacle()
{
	int sizeOffset{ CELL_SIZE / 2 };	// 위치를 맞추기 위한 값
	// 가로
	for (int i = 0; i < BOARD_SIZE; ++i) {
		if (i >= 7 && i <= 9) continue;
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ ((float)gBackgroundRect.left + sizeOffset) + i * CELL_SIZE, (float)gBackgroundRect.top + sizeOffset }));
	}
	for (int i = 0; i < BOARD_SIZE; ++i) {
		if (i >= 7 && i <= 9) continue;
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + sizeOffset + i * CELL_SIZE, (float)gBackgroundRect.top + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE }));
	}
	// 세로
	for (int i = 1; i < BOARD_SIZE - 1; ++i) {
		if (i >= 7 && i <= 9) continue;
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + sizeOffset, (float)gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
	}
	for (int i = 1; i < BOARD_SIZE - 1; ++i) {
		if (i >= 7 && i <= 9) continue;
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + sizeOffset + (BOARD_SIZE - 1) * CELL_SIZE, (float)gBackgroundRect.top + sizeOffset + i * CELL_SIZE }));
	}

	switch (_curStage) {
	case 1:
		break;
	case 2:
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
		break;
	case 3:
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset }));

		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 5 * CELL_SIZE + sizeOffset }));

		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 5 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));

		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 11 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 11 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
		break;
	case 4:
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.right - 4 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.bottom - 4 * CELL_SIZE + sizeOffset }));

		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 6 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 6 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
		_obstacles.push_back(std::make_shared<GameObject>(ObjectType::Obstacle, Vertex{ (float)gBackgroundRect.left + 10 * CELL_SIZE + sizeOffset, (float)gBackgroundRect.top + 10 * CELL_SIZE + sizeOffset }));
		break;
	}
}

void GameScene::AddPlayer(int id, PlayerRef player)
{		
	EnterCriticalSection(&g_cs);

	// MyPlayer 설정
	if (!_localPlayer)
	{
		_localPlayer = player;
		_localPlayer->SetId(id);

		LeaveCriticalSection(&g_cs);
		return;
	}

	// 다른 Player 설정
	if (id == _localPlayer->GetId())	// 이거 안 하면 클라 두개 실행시 첫 번째 클라에서 플레이어 3명 그려짐
	{
		LeaveCriticalSection(&g_cs);
		return;
	}

	_players[id] = player;
	_players[id]->SetId(id);

	LeaveCriticalSection(&g_cs);
}

void GameScene::AddMonster(int id, MonsterRef monster)
{
	EnterCriticalSection(&g_cs);
	_monsters[id] = monster;
	_monsters[id]->SetId(id);
	LeaveCriticalSection(&g_cs);
}

void GameScene::AddObject(int id, GameObjectRef object)
{
	EnterCriticalSection(&g_cs);
	_objects[id] = object;
	_objects[id]->SetId(id);
	LeaveCriticalSection(&g_cs);
}

void GameScene::AddEndGameUi(bool isStay, Vertex center, Vertex size, std::wstring text)
{
	DWORD StayButtonColor{ RGB(51, 102, 255) };
	DWORD LeaveButtonColor{ RGB(255, 102, 153) };

	EnterCriticalSection(&g_cs);
	if (isStay)
		_stayButton = new Button(center, size, text, StayButtonColor);
	else
		_leaveButton = new Button(center, size, text, LeaveButtonColor);
	LeaveCriticalSection(&g_cs);

	_isStayButtonActive = true;
}

Dir GameScene::ConvertVecToDir(const Vertex& dir)
{
	// 상하좌우
	if (dir.x == -1 && dir.y == 0) return Dir::Left;
	if (dir.x == 1 && dir.y == 0) return Dir::Right;
	if (dir.x == 0 && dir.y == -1) return Dir::Up;
	if (dir.x == 0 && dir.y == 1) return Dir::Down;

	// 대각선
	if (dir.x == -1 && dir.y == -1) return Dir::LeftUp;
	if (dir.x == -1 && dir.y == 1) return Dir::LeftDown;
	if (dir.x == 1 && dir.y == -1) return Dir::RightUp;
	if (dir.x == 1 && dir.y == 1) return Dir::RightDown;
}

void GameScene::SyncPlayer(int id, const Vertex& pos, const Dir dir, const ObjectState state)
{
	EnterCriticalSection(&g_cs);
	if (_localPlayer->GetId() == id) {
		_localPlayer->SetPos(pos);
		_localPlayer->SetDirAndFrame(dir);
		_localPlayer->SetState(state);
	}
	if (_players.find(id) != _players.end()) 
	{
		_players[id]->SetPos(pos);
		_players[id]->SetDirAndFrame(dir);
		_players[id]->SetState(state);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::SyncMonster(int id, const Vertex& pos, const Dir dir, const ObjectState state)
{
	EnterCriticalSection(&g_cs);
	if (_monsters.find(id) != _monsters.end())
	{	
		_monsters[id]->SetPos(pos);
		_monsters[id]->SetDirAndFrame(dir);
		_monsters[id]->SetState(state);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::SyncBullet(int id, const Vertex& pos)
{
	EnterCriticalSection(&g_cs);
	if (_objects.find(id) != _objects.end())
	{	
		_objects[id]->SetPos(pos);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::GetItemLocalPlayer(ItemType itemType)
{
	EnterCriticalSection(&g_cs);
	_localPlayer->SetItem(itemType);
	LeaveCriticalSection(&g_cs);
}

void GameScene::SyncObjectTimer(const int timer, const int id)
{
	EnterCriticalSection(&g_cs);
	if (_objects.find(id) != _objects.end())
	{
		_objects[id]->SetTimer(timer);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::ProcessInput()
{
	EnterCriticalSection(&g_cs);
	if (!_localPlayer)
	{
		LeaveCriticalSection(&g_cs);
		return;
	}
	LeaveCriticalSection(&g_cs);

	// 코드 길어져서 포인터로 받기
	InputManager* input = GET_SINGLE(InputManager);

	static bool prevKeyUp{};

	// 이동
	Vertex vecDir{};
	if (input->GetButton(KeyType::A)) vecDir.x -= 1;
	if (input->GetButton(KeyType::D)) vecDir.x += 1;
	if (input->GetButton(KeyType::W)) vecDir.y -= 1;
	if (input->GetButton(KeyType::S)) vecDir.y += 1;

	Dir dir = ConvertVecToDir(vecDir);
	bool moved = false;

	if (vecDir.x != 0 || vecDir.y != 0) {
		EnterCriticalSection(&g_cs);
		_localPlayer->Move(vecDir, dir);
		LeaveCriticalSection(&g_cs);
		moved = true;	}

	if (moved)
	{
		EnterCriticalSection(&g_cs);
		Vertex curPos = _localPlayer->GetPos();
		Vertex prevPos = _localPlayer->GetPrevSendPos();
		LeaveCriticalSection(&g_cs);

		int dx = abs(static_cast<int>(curPos.x) - static_cast<int>(prevPos.x));
		int dy = abs(static_cast<int>(curPos.y) - static_cast<int>(prevPos.y));

		if (dx > 1 || dy > 1)
		{
			EnterCriticalSection(&g_cs);
			// Player 내부 변수 갱신
			_localPlayer->SetPrevSendPos(curPos);
			_localPlayer->SetPrevDir(dir);

			// 서버로 이동 패킷 전송
			_gameNetwork->SendMovePacket(_localPlayer->GetId(), ObjectType::Player, curPos, dir, _localPlayer->GetState());
			LeaveCriticalSection(&g_cs);
		}
	}

	// 총알 발사
	{
		bool downUp = input->GetButtonDown(KeyType::Up);
		bool downDown = input->GetButtonDown(KeyType::Down);
		bool downLeft = input->GetButtonDown(KeyType::Left);
		bool downRight = input->GetButtonDown(KeyType::Right);

		if (downUp || downDown || downLeft || downRight)
		{
			Vertex playerPos = _localPlayer->GetPos();
			Dir shootDir;

			// 1) 대각선 먼저 체크
			if (downUp && downRight) {
				shootDir = Dir::RightUp;
			}
			else if (downUp && downLeft) {
				shootDir = Dir::LeftUp;
			}
			else if (downDown && downRight) {
				shootDir = Dir::RightDown;
			}
			else if (downDown && downLeft) {
				shootDir = Dir::LeftDown;
			}
			// 2) 단일 방향
			else if (downUp) {
				shootDir = Dir::Up;
			}
			else if (downDown) {
				shootDir = Dir::Down;
			}
			else if (downLeft) {
				shootDir = Dir::Left;
			}
			else if (downRight) {
				shootDir = Dir::Right;
			}

			// 아이템 사용 여부에 따라 발사 방식 분리
			if (useWaterWheel) {
				// 아이템 사용 시: 8방향 발사
				for (int i = 0; i < 8; ++i) {
					EnterCriticalSection(&g_cs);
					_gameNetwork->SendCreateProjectilePacket(_localPlayer->GetId(), playerPos, Dir(i));
					LeaveCriticalSection(&g_cs);
				}
			}
			else if (useShotgun) {
				// 샷건 모드: 기준 방향 중심으로 3방향 발사
				ShotGunFire(shootDir);
			}
			else {
				// 평소: 선택된 방향으로 1발만 발사
				EnterCriticalSection(&g_cs);
				_gameNetwork->SendCreateProjectilePacket(_localPlayer->GetId(), playerPos, shootDir);
				LeaveCriticalSection(&g_cs);
			}
		}
	}

	// 아이템 사용
	if (input->GetButtonDown(KeyType::SpaceBar)) {
		EnterCriticalSection(&g_cs);
		ItemRef item = _localPlayer->GetItem();
		LeaveCriticalSection(&g_cs);

		if (item) {
			EnterCriticalSection(&g_cs);
			_gameNetwork->SendUseItemPacket(_localPlayer->GetId(), _localPlayer->GetObjectType(), item->GetItemType());
			LeaveCriticalSection(&g_cs);
		}
	}
	
	// 폭탄 발로 차기
	if (input->GetButtonDown(KeyType::LeftShift))
	{
		EnterCriticalSection(&g_cs);
		_gameNetwork->SendKickBombPacket(_localPlayer->GetId(), _localPlayer->GetDir());
		LeaveCriticalSection(&g_cs);
	}

		// 
		//if (prevKeyUp || CheckTimer(_localPlayer->_timer, bulletSpeed)) {
		//	Vertex playerPos = _localPlayer->GetPos();
		//	Dir shootDir = Dir::Down;

		//	bool up = input->GetButton(KeyType::Up);
		//	bool down = input->GetButton(KeyType::Down);
		//	bool left = input->GetButton(KeyType::Left);
		//	bool right = input->GetButton(KeyType::Right);

		//	if (up)
		//	{
		//		if (right) shootDir = Dir::RightUp;
		//		else if (left) shootDir = Dir::LeftUp;
		//		else shootDir = Dir::Up;
		//	}
		//	else if (down)
		//	{
		//		if (right) shootDir = Dir::RightDown;
		//		else if (left) shootDir = Dir::LeftDown;
		//		else shootDir = Dir::Down;
		//	}
		//	else if (left)
		//	{
		//		shootDir = Dir::Left;
		//	}
		//	else if (right)
		//	{
		//		shootDir = Dir::Right;
		//	}

		//	// 방향에 따른 총알 생성 in Local - 생성을 서버에서 받은 후에 해야하나???
		//	switch (shootDir)
		//	{
		//	case Dir::Up:
		//		//_objects.push_back(std::make_shared<Projectile>(Dir::Up, playerPos));
		//		if (useShotgun) {
		//			//_objects.push_back(std::make_shared<Projectile>(Dir::RightUp, playerPos));
		//			//_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
		//		}
		//		prevKeyUp = false;
		//		_gameNetwork->SendCreateProjectilePacket(_localPlayer->GetId(), playerPos, shootDir);
		//		break;
		//	}
		//
		//else if (input->GetButton(KeyType::Left)) {
		//	_objects.push_back(std::make_shared<Projectile>(Dir::Left, playerPos));
		//	if (useShotgun) {
		//		_objects.push_back(std::make_shared<Projectile>(Dir::LeftUp, playerPos));
		//		_objects.push_back(std::make_shared<Projectile>(Dir::LeftDown, playerPos));
		//	}
		//	prevKeyUp = false;
		//}

		// 서버로 Create Projectile 패킷 Send
		//_gameNetwork->SendCreateProjectilePacket(_localPlayer->GetId(), playerPos, shootDir);



	// 이동 키 Up
	/*if (input->GetButtonUp(KeyType::W) || input->GetButtonUp(KeyType::A) || input->GetButtonUp(KeyType::S) || input->GetButtonUp(KeyType::D)) {
		
		_localPlayer->ResetCurFrame();
		_localPlayer->SetState(ObjectState::Idle);	
		
	}
	if (input->GetButtonUp(KeyType::Left) || input->GetButtonUp(KeyType::Up) || input->GetButtonUp(KeyType::Down) || input->GetButtonUp(KeyType::Right)) {
		prevKeyUp = true;
		_localPlayer->_timer = 0.0f;
		}*/

	//// 버튼 클릭
	//if (input->GetButtonDown(KeyType::LeftMouse)) {
	//	for (const auto& button : _ui) {
	//		if (button->GetObjectType() == ObjectType::Button && button->Intersects(input->GetMousePos())) {
	//			_monsters.push_back(std::make_shared<TankMonster>());	// test
	//		}
	//	}
	//}

	//// 폭탄 발로 차기
	//if (input->GetButtonDown(KeyType::LeftShift)) {
	//	for (const auto& object : _objects) {
	//		if (object->GetObjectType() == ObjectType::Bomb && !dynamic_cast<BombObject*>(object.get())->_isBomb && _localPlayer->IsCollision(object.get())) {
	//			object->SetDir(_localPlayer->GetDir());
	//			for (int i = 0; i < BOMB_MOVE; ++i) {
	//				object->Move(); // CELLSIZE만큼씩 이동
	//				for (const auto& otherObject : _objects) {	// 장애물에 걸리면 이동X
	//					if (otherObject->GetObjectType() == ObjectType::Obstacle && object->IsCollision(otherObject.get())) {
	//						object->UndoPos();
	//						i = BOMB_MOVE;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

}

void GameScene::ShotGunFire(Dir baseDir) {
	Vertex playerPos = _localPlayer->GetPos();

	std::vector<Dir> dirs;

	switch (baseDir) {
	case Dir::Right:
		dirs = { Dir::Right, Dir::RightUp, Dir::RightDown };
		break;
	case Dir::Left:
		dirs = { Dir::Left, Dir::LeftUp, Dir::LeftDown };
		break;
	case Dir::Up:
		dirs = { Dir::Up, Dir::LeftUp, Dir::RightUp };
		break;
	case Dir::Down:
		dirs = { Dir::Down, Dir::LeftDown, Dir::RightDown };
		break;
	case Dir::RightUp:
		dirs = { Dir::RightUp, Dir::Right, Dir::Up };
		break;
	case Dir::RightDown:
		dirs = { Dir::RightDown, Dir::Right, Dir::Down };
		break;
	case Dir::LeftUp:
		dirs = { Dir::LeftUp, Dir::Left, Dir::Up };
		break;
	case Dir::LeftDown:
		dirs = { Dir::LeftDown, Dir::Left, Dir::Down };
		break;
	}

	for (Dir d : dirs) {
		EnterCriticalSection(&g_cs);
		_gameNetwork->SendCreateProjectilePacket(_localPlayer->GetId(), playerPos, d);
		LeaveCriticalSection(&g_cs);
	}
}
