#include "Header.h"

// 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �ð�

FMOD::System* ssystem;
FMOD::Sound* main_music;
FMOD::Sound* stage_music;
FMOD::Sound* boss_music;
FMOD::Sound* bullet_sound;
FMOD::Sound* hit_sound;
FMOD::Sound* boom_sound;
FMOD::Sound* item_get_sound;
FMOD::Sound* item_use_sound;
FMOD::Sound* gameclear_sound;
FMOD::Channel* short_ch = 0;
FMOD::Channel* long_ch = 0;
FMOD::Channel* boom_ch = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

HWND _hWnd;

// ��Ʈ��
HBITMAP wall_wood_block;
HBITMAP wood_block;
HBITMAP wood_block_mask;
HBITMAP hStageBitmap[5];
HBITMAP hCellBitmap = NULL;
HBITMAP hObstacleBitmap = NULL;

HBITMAP cowboy, cowboy_mask;
HBITMAP item_image, item_mask;

HBITMAP move_obstacle;
HBITMAP move_obstacle_mask;

HBITMAP boss_sprite, boss_mask;

// ����Ʈ ��Ʈ��
HBITMAP coffee_effect;
HBITMAP bullet_effect, bullet_effect_mask;
HBITMAP shotgun_effect, shotgun_effect_mask;
HBITMAP waterwheel_effect, waterwheel_effect_mask;
HBITMAP hourglass_effect;
HBITMAP lightning_effect, lightning_effect_mask, lightning_effect_background;

// ���� ��Ʈ��
HBITMAP normal_monster;
HBITMAP tank_monster;
HBITMAP respawn_monster;
HBITMAP obstacle_monster;
HBITMAP boomer_monster;

HBITMAP normal_monster_mask;
HBITMAP tank_monster_mask;
HBITMAP respawn_monster_mask;
HBITMAP obstacle_monster_mask;
HBITMAP boomer_monster_mask;

HBITMAP bomb;
HBITMAP bomb_mask;
HBITMAP bombing;
HBITMAP bombing_mask;

HBITMAP dead_effect;
HBITMAP dead_effect_mask;

int state = START_MENU;

// ��ü ���� ����
PLAYER player;
BULLET* bullet;
ITEM item[ITEM_NUM];
ITEMNUM item_num;
BOSS boss;
BOSSGUN* bossgun;

// ���� ������ �迭 ����
SHOPITEM itemList[] = {
	{L"�Ź�", 8},
	{ L"�Ѿ�", 10},
	{ L"��", 15}
};

// ���� ��ü ���� ����
MONSTERNODE* monster_head;
// ��Ȱ ���� ��ü ���� ����
MONSTERNODE* respawn_head;
// ��ź ��ü ���� ����
BOMBNODE* bomb_head;
// ���� ��ü ���� ����
BOMBNODE* bombing_head;
// ��� ����Ʈ ��ü ���� ����
BOMBNODE* dead_head;
// ������ ��ֹ� ��ü
OBSTACLE* obstacles;
int obstacle_num;

// �÷��̾� ������
int frame_numW, frame_numL;

// �Ѿ� ����
int bullet_num = 0;
int bullet_speed = 15;
int bullet_count = 0;

// �������� ����
int currentStage = 1;
int stageIndex = 0;     
int animationOffset = 0;


// ���� ���� ����
int bossgun_num = 0;
int bigbossgun_num = 0;
int bossframe_numL, bossframe_numW;

// �� ����, �� ��ǥ
int StartX = 130, StartY = 40, EndX = 610, EndY = 520;

// ���� ���� ����
MERCHANT merchant;

const int walkAnimation[MERCHANT_SPRITE_CNT] = { 0, 1, 2, 3 };

// ������ ���� �ִ��� ���θ� �����ϴ� ����
bool isShopOpen = false;

// ������ ���� ���� ���θ� �����ϴ� ����
bool isEntering = false;
bool isExit = false;

// ���� �Ϸ� ���θ� �����ϴ� ����
bool isPurchaseComplete = false;
int play_time;

// ���� ������������ ���θ� �����ϴ� ����
//bool isBossStage = false;
bool moveleft = GetAsyncKeyState(VK_LEFT) & 0x8000, moveright = GetAsyncKeyState(VK_RIGHT) & 0x8000;

// ----------------------------------------------------- //
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"CowBoy";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassExW(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

// ----------------------------------------------------- //
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static RECT rt;
	static POINT m;
	static int Lframe, Rframe, Uframe, Dframe;
	static int LUframe, LDframe, RUframe, RDframe;
	static RECT menu_start_rt;
	static RECT menu_exit_rt;
	_hWnd = hWnd;

	switch (uMsg) {
	case WM_CREATE:
		// ������ ũ�� ����
		GetClientRect(hWnd, &rt);
		// �����Լ� �ߺ� ����
		srand(time(NULL));
		menu_start_rt = { rt.right / 5, rt.bottom / 2 - FONT_SIZE,  rt.right / 5 * 2, rt.bottom / 2 };
		menu_exit_rt = { menu_start_rt.left, menu_start_rt.top + FONT_SIZE * 2, menu_start_rt.right, menu_start_rt.bottom + FONT_SIZE * 2 };
		EffectBitmapLoad();
		InitMerchant();
		// ���� ����
		SoundSetting();
		// ��� ���
		long_ch->stop();
		ssystem->playSound(main_music, 0, false, &long_ch);
		long_ch->setVolume(0.3);
		break;
	case WM_LBUTTONDOWN:
		if (START_MENU) {
			m.x = LOWORD(lParam);
			m.y = HIWORD(lParam);

			if (Menu_Is_Click(m, menu_start_rt, menu_exit_rt)) {
				long_ch->stop();
				ssystem->playSound(stage_music, 0, false, &long_ch);
				long_ch->setVolume(0.3);
				state = IN_GAME;
				// �÷��̾� �ʱ� ����
				InitPlayer(hWnd);

				// ���� ���� Ÿ�̸� ����
				SetTimer(hWnd, STAGE_START, 1000, NULL);
				// ���� ���� Ÿ�̸� ����
				SetTimer(hWnd, MONSTER_PRODUSE, 3000, NULL);
				// ���� �̵� Ÿ�̸� ����
				SetTimer(hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				obstacles = InitObstalce(obstacles);
				InvalidateRect(hWnd, NULL, true);
			}
		}
		break;
	case WM_TIMER:
		switch (wParam)	{
		case STAGE_START:
			play_time++;
			if (play_time >= STAGE_TIME) {
				KillTimer(hWnd, STAGE_START);
				KillTimer(hWnd, MONSTER_PRODUSE);
			}
			break;
		case MONSTER_PRODUSE:
			MONSTER data;
			initMonster(&data, player);
			InsertMonster(&monster_head, data);
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		// �̵� �� Ű�ٿ� ����
		if (animationOffset != 0) {
			break;
		}
		// �÷��̾� �̵�
		if ((GetKeyState(VK_LEFT) < 0) && (GetKeyState(VK_UP) >= 0) && (GetKeyState(VK_DOWN) >= 0))
		{
			Lframe++;
			player.P.x -= PLAYER_SPEED * player.speed;
			frame_numW = (Lframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 1;
			// �÷��̾� ��ֹ� �浹 üũ
			if (PlayerBlockObject()) {
				player.P.x += PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_RIGHT) < 0) && (GetKeyState(VK_UP) >= 0) && (GetKeyState(VK_DOWN) >= 0))
		{
			Rframe++;
			player.P.x += PLAYER_SPEED * player.speed;
			frame_numW = (Rframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 0;
			if (PlayerBlockObject()) {
				player.P.x -= PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_UP) < 0) && (GetKeyState(VK_LEFT) >= 0) && (GetKeyState(VK_RIGHT) >= 0))
		{
			Uframe++;
			player.P.y -= PLAYER_SPEED * player.speed;
			frame_numW = (Uframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 5;
			if (PlayerBlockObject()) {
				player.P.y += PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_DOWN) < 0) && (GetKeyState(VK_LEFT) >= 0) && (GetKeyState(VK_RIGHT) >= 0))
		{
			Dframe++;
			player.P.y += PLAYER_SPEED * player.speed;
			frame_numW = (Dframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 9;
			if (PlayerBlockObject()) {
				player.P.y -= PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_LEFT) < 0) && (GetKeyState(VK_UP) < 0))
		{
			LUframe++;
			player.P.x -= PLAYER_SPEED * player.speed;
			player.P.y -= PLAYER_SPEED * player.speed;
			frame_numW = (LUframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 6;
			if (PlayerBlockObject()) {
				player.P.x += PLAYER_SPEED * player.speed;
				player.P.y += PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_LEFT) < 0) && (GetKeyState(VK_DOWN) < 0))
		{
			LDframe++;
			player.P.x -= PLAYER_SPEED * player.speed;
			player.P.y += PLAYER_SPEED * player.speed;
			frame_numW = (LDframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 8;
			if (PlayerBlockObject()) {
				player.P.x += PLAYER_SPEED * player.speed;
				player.P.y -= PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_RIGHT) < 0) && (GetKeyState(VK_UP) < 0))
		{
			RUframe++;
			player.P.x += PLAYER_SPEED * player.speed;
			player.P.y -= PLAYER_SPEED * player.speed;
			frame_numW = (RUframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 4;
			if (PlayerBlockObject()) {
				player.P.x -= PLAYER_SPEED * player.speed;
				player.P.y += PLAYER_SPEED * player.speed;
			}
		}
		else if ((GetKeyState(VK_RIGHT) < 0) && (GetKeyState(VK_DOWN) < 0))
		{
			RDframe++;
			player.P.x += PLAYER_SPEED * player.speed;
			player.P.y += PLAYER_SPEED * player.speed;
			frame_numW = (RDframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
			frame_numL = 2;
			if (PlayerBlockObject()) {
				player.P.x -= PLAYER_SPEED * player.speed;
				player.P.y -= PLAYER_SPEED * player.speed;
			}
		}

		// �Ѿ� �߻�
		if ((GetKeyState(VK_F2) < 0) && (GetKeyState('1') >= 0) && (GetKeyState('3') >= 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(1, 2, 8);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(1);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(1);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState('1') < 0) && (GetKeyState(VK_F2) >= 0) && (GetKeyState('2') >= 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(6, 7, 8);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(7);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(7);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState('2') < 0) && (GetKeyState('1') >= 0) && (GetKeyState('3') >= 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(4, 5, 6);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(5);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(5);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState('3') < 0) && (GetKeyState(VK_F2) >= 0) && (GetKeyState('2') >= 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(2, 3, 4);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(3);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(3);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState(VK_F2) < 0) && (GetKeyState('1') < 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(1, 7, 8);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(8);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(8);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState(VK_F2) < 0) && (GetKeyState('3') < 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(1, 2, 3);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(2);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(2);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState('2') < 0) && (GetKeyState('1') < 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(5, 6, 7);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(6);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(6);
				}
			}
			bullet_count++;
		}
		else if ((GetKeyState('2') < 0) && (GetKeyState('3') < 0))
		{
			if (bullet_count % bullet_speed == 0)
			{
				if (item[player.item_idx].effect) {
					if (player.bullet && item[player.item_idx].num == 6) {
						ShotgunBullet(3, 4, 5);
					}
					else if (player.bullet && item[player.item_idx].num == 7) {
						WaterWheelBullet();
					}
					else {
						MakeBullet();
						SetDirect(4);
					}
				}
				else
				{
					MakeBullet();
					SetDirect(4);
				}
			}
			bullet_count++;
		}

		if (wParam == VK_SHIFT)
			KickBomb();
		
		// ������ ���
		if (wParam == VK_SPACE)
			UseItem(hWnd);

		
		// �÷��̾�, ������ �浹üũ
		for (int i = 0; i < ITEM_NUM; ++i) {
			if (item[i].show) {
				item[i] = GetItem(player.P.x, player.P.y, item[i], i);
			}
		}

		if (isShopOpen)
		{
			for (int i = 0; i < sizeof(itemList) / sizeof(itemList[0]); i++)
				GetShopItem(player.P.x, player.P.y, itemList[i]);
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_KEYUP:
		// Ű���� ���� ���ְ� �ϱ�
		if (wParam == VK_LEFT)
		{
			Rframe = Uframe = Dframe = Lframe = 0;
			frame_numW = COWBOY_SPRITE_WCNT * 1;
		}
		else if (wParam == VK_RIGHT)
		{
			Rframe = Uframe = Dframe = Lframe = 0;
			frame_numW = COWBOY_SPRITE_WCNT * 0;
		}
		else if (wParam == VK_UP)
		{
			Rframe = Uframe = Dframe = Lframe = 0;
			frame_numW = COWBOY_SPRITE_WCNT * 5;
		}
		else if (wParam == VK_DOWN)
		{
			Rframe = Uframe = Dframe = Lframe = 0;
			frame_numW = COWBOY_SPRITE_WCNT * 9;
		}
		// �Ѿ� �ӵ� ī��Ʈ �ʱ�ȭ
		if (wParam == VK_F2)
			bullet_count = 0;
		else if (wParam == '1')
			bullet_count = 0;
		else if (wParam == '2')
			bullet_count = 0;
		else if (wParam == '3')
			bullet_count = 0;
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if(state == START_MENU)
			StartMenu(hDC, rt, menu_start_rt, menu_exit_rt);
		else if(IN_GAME){
			// ������ �־ �浹üũ
			PlayerBlockObject();

			if (play_time >= STAGE_TIME) {
				if(!monster_head && !respawn_head)
					if(!isEntering)
						EndStage();
			}
			GameUI(player, hDC);

			// ��Ʈ�� ���
			DrawBitmap(hDC, rt);
		}

		ReleaseDC(hWnd, hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		ssystem->release();
		ssystem->close();
	
		for (int i = 1; i <= BOSS_OBSTACLE; ++i) {
			KillTimer(hWnd, i);
		}

		DeleteObject(coffee_effect);
		DeleteObject(bullet_effect);
		DeleteObject(bullet_effect_mask);
		DeleteObject(lightning_effect);
		DeleteObject(lightning_effect_mask);
		DeleteObject(lightning_effect_background);
		DeleteObject(shotgun_effect);
		DeleteObject(shotgun_effect_mask);
		DeleteObject(waterwheel_effect);
		DeleteObject(waterwheel_effect_mask);
		DeleteObject(hourglass_effect);

		if (monster_head) free(monster_head);
		if (respawn_head) free(respawn_head);
		if (bomb_head) free(bomb_head);
		if (bombing_head) free(bombing_head);
		if (dead_head) free(dead_head);
		if (obstacles) free(obstacles);

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SoundSetting(void)
{
	// ���� �ý��� ����
	result = FMOD::System_Create(&ssystem);
	if (result != FMOD_OK)
		exit(0);

	// ���� �ý��� �ʱ�ȭ
	ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata);

	// ���� ���� �� ����
	ssystem->createSound("main_music.mp3", FMOD_LOOP_NORMAL, 0, &main_music);
	ssystem->createSound("stage_music.mp3", FMOD_LOOP_NORMAL, 0, &stage_music);
	ssystem->createSound("boss_music.mp3", FMOD_LOOP_NORMAL, 0, &boss_music);
	ssystem->createSound("bullet_sound.mp3", FMOD_DEFAULT, 0, &bullet_sound);
	ssystem->createSound("hit_sound.mp3", FMOD_DEFAULT, 0, &hit_sound);
	ssystem->createSound("boom_sound.mp3", FMOD_DEFAULT, 0, &boom_sound);
	ssystem->createSound("item_get_sound.mp3", FMOD_DEFAULT, 0, &item_get_sound);
	ssystem->createSound("item_use_sound.mp3", FMOD_DEFAULT, 0, &item_use_sound);
	ssystem->createSound("gameclear_sound.mp3", FMOD_DEFAULT, 0, &gameclear_sound);
}