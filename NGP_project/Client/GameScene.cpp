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
	GET_SINGLE(SoundManager)->Play(L"main_music", true);

	// UI
	_ui.push_back(std::make_shared<UI>(Vertex{ 70, 100 }, Vertex{100, 100}));
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
	for (const auto& monster : _monsters) {
		monster.second->Update();
	}
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
	// Obstacle 
	EnterCriticalSection(&g_cs);
	for (const auto obstacle : _obstacles)
	{
		obstacle->Render(memDC, memDCImage);
	}
	LeaveCriticalSection(&g_cs);

	// UI
	EnterCriticalSection(&g_cs);
	if (_localPlayer.get()) {
		for (const auto ui : _ui) {
			ui->Render(memDC, _localPlayer->_status._life);	// 나중에 수정
		}
	}
	LeaveCriticalSection(&g_cs);

	// Local Player
	EnterCriticalSection(&g_cs);
	if (_localPlayer) {
		if (_localPlayer->GetState() != ObjectState::Dead)
		{
			_localPlayer->Render(memDC, memDCImage);
			_localPlayer->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 0, 0));
		}
	}
	LeaveCriticalSection(&g_cs);

	// Other Player
	EnterCriticalSection(&g_cs);
	for (const auto& [id, player] : _players) {
		if (player->GetState() != ObjectState::Dead)
		{
			player->Render(memDC, memDCImage);
			//player->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 255, 0));	// 디버깅용
		}
	}
	LeaveCriticalSection(&g_cs);

	// Monster
	EnterCriticalSection(&g_cs);
	for (const auto& [id, monster] : _monsters) {
		monster->Render(memDC, memDCImage);
		//monster->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 0, 0));	// 디버깅용
	}
	LeaveCriticalSection(&g_cs);

	// Objects
	EnterCriticalSection(&g_cs);
	for (const auto& [id, object] : _objects) {
		object->Render(memDC, memDCImage);
		//object->GetBoundingBox().Render(memDC, memDCImage, RGB(0, 0, 0));	// 디버깅용
	}
	LeaveCriticalSection(&g_cs);

	EnterCriticalSection(&g_cs);
	if ( _isStayButtonActive && _stayButton && _leaveButton)
	{
		_stayButton->Render(memDC, 0);
		_leaveButton->Render(memDC, 0);
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
	if (id == _localPlayer->GetId())
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

void GameScene::RemovePlayer(int id)
{
	EnterCriticalSection(&g_cs);
	_players.erase(id);
	LeaveCriticalSection(&g_cs);
}

void GameScene::RemoveMonster(int id)
{
	EnterCriticalSection(&g_cs);
	_monsters.erase(id);
	LeaveCriticalSection(&g_cs);
}

void GameScene::RemoveObject(int id)
{
	EnterCriticalSection(&g_cs);
	_objects.erase(id);
	LeaveCriticalSection(&g_cs);
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
	}
	if (_players.find(id) != _players.end()) 
	{
		_players[id]->SetPos(pos);
		_players[id]->SetDirAndFrame(dir);
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
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::SyncObject(int id, const Vertex& pos)
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

void GameScene::SetPlayerState(int id, const ObjectState state)
{
	EnterCriticalSection(&g_cs);
	if (_localPlayer->GetId() == id) {
		_localPlayer->SetState(state);
	}
	if (_players.find(id) != _players.end())
	{
		_players[id]->SetState(state);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::SetMonsterState(int id, const ObjectState state)
{
	EnterCriticalSection(&g_cs);
	if (_monsters.find(id) != _monsters.end())
	{
		_monsters[id]->SetState(state);
	}
	LeaveCriticalSection(&g_cs);
}

void GameScene::SetLifeOfLocalPlayer(int life)
{
	EnterCriticalSection(&g_cs);
	_localPlayer->_status._life = life;
	LeaveCriticalSection(&g_cs);
}

void GameScene::SetObjectState(int id, const ObjectState state)
{
	EnterCriticalSection(&g_cs);
	if (_objects.find(id) != _objects.end())
	{
		_objects[id]->SetState(state);
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
