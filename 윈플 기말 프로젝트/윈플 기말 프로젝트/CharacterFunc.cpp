#include "Header.h"

extern HINSTANCE g_hInst;
extern HWND _hWnd;

// ����
extern FMOD::System* ssystem;
extern FMOD::Sound* main_music;
extern FMOD::Sound* stage_music;
extern FMOD::Sound* boss_music;
extern FMOD::Sound* bullet_sound;
extern FMOD::Sound* hit_sound;
extern FMOD::Sound* boom_sound;
extern FMOD::Sound* item_get_sound;
extern FMOD::Sound* item_use_sound;
extern FMOD::Sound* gameclear_sound;
extern FMOD::Channel* short_ch;
extern FMOD::Channel* long_ch;
extern FMOD::Channel* boom_ch;

extern int frame_numW, frame_numL;
extern PLAYER player;

extern BULLET* bullet;
extern int bullet_num;
extern int bullet_speed;

extern ITEM item[ITEM_NUM];
extern ITEMNUM item_num;
extern MERCHANT merchant;
extern SHOPITEM itemList[];

extern MONSTERNODE* monster_head;
extern MONSTERNODE* respawn_head;
extern BOMBNODE* bomb_head;
extern BOMBNODE* bombing_head;
extern BOMBNODE* dead_head;
extern OBSTACLE* obstacles;
extern int obstacle_num;

extern int currentStage;
extern int animationOffset;
extern int stageIndex;
extern HBITMAP hStageBitmap[5];   // �������� ��Ʈ�� ����
extern int play_time;

extern bool isEntering;
extern bool isShopOpen;
extern bool isExit;
extern bool isPurchaseComplete;
//extern bool isBossStage;

extern int state;


extern int bossframe_numW, bossframe_numL;
extern BOSS boss;
extern BOSSGUN* bossgun;
extern int bossgun_num;
extern int bigbossgun_num;
extern int StartX, StartY, EndX, EndY;
extern int bossleft, bossright;
extern bool moveleft, moveright;

// ���� ID
int normal_id;
int respawn_id;
int tank_id;
int onstacle_id;
int boomer_id;

int clear = 0;

// ����
RECT GetStageRect(int x, int y)
{
	return { x, y, x + CELL_SIZE, y + CELL_SIZE };
}

void EndStage()
{
	isEntering = true;

	SetTimer(_hWnd, MERCHANT_WALKING, 100, reinterpret_cast<TIMERPROC>(MerchantWalking));
}

void TransitionToNextStage(HWND hWnd)
{
	isPurchaseComplete = true;

	if (currentStage == STAGE_COUNT)
	{
		// ���� ���������� ������ ���������� ��� ó��
		return;
	}

	if (currentStage == 4)
	{
		long_ch->stop();
		ssystem->playSound(boss_music, 0, false, &long_ch);
		long_ch->setVolume(0.3);
		InitBoss(hWnd);

		KillTimer(hWnd, STAGE_START);
		// ���� Ÿ�̸� ����
		SetTimer(hWnd, BOSS_MOVE, 120, reinterpret_cast<TIMERPROC>(BossFollowPlayer));
		//// ���� �Ѿ� �߻� Ÿ�̸� ����
		//SetTimer(hWnd, BOSS_SHOOT, 3000, reinterpret_cast<TIMERPROC>(Shooting));
		// ��ֹ� ��ġ Ÿ�̸�
		//SetTimer(hWnd, BOSS_OBSTACLE, 1000, reinterpret_cast<TIMERPROC>(ObstacleInstallation));
	}

	// ���ΰ��� ���������� �Ʒ��� �������鼭 ���� ���������� ��ȯ
	int animationFrames = BOARD_SIZE;  // �ִϸ��̼� ������ ��

	// �ִϸ��̼��� ���� Ÿ�̸� ����
	SetTimer(hWnd, STAGE_ANIMATION, ANIMATION_SPEED, reinterpret_cast<TIMERPROC>(AnimateStageTransition));

	// �ִϸ��̼��� ���� ��ȯ ���� ����
	stageIndex = currentStage;  // ���� �������� �ε��� ����

	// ȭ�� ����
	InvalidateRect(hWnd, NULL, TRUE);
}


void CALLBACK AnimateStageTransition(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	static bool is_first = true;
	static int Dframe = 0;
	// �ִϸ��̼� ������ ���� ���
	int animationFrames = BOARD_SIZE;  // �ִϸ��̼� ������ ��
	int frameHeight = (BOARD_SIZE * CELL_SIZE) / animationFrames;

	// �ִϸ��̼� ������ ����
	animationOffset += frameHeight;

	if (is_first) {

		player.P.y = 0;
		is_first = false;
	}
	else {
		Dframe++;
		// ���ΰ��� ��ġ�� �Ʒ��� �̵�
		player.P.y += PLAYER_SPEED;
		frame_numW = (Dframe) % COWBOY_SPRITE_WCNT + COWBOY_SPRITE_WCNT;
		frame_numL = 9;
	}

	// ���� ���������� ��Ʈ�� �׸���
	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hStageBitmap[currentStage + 1]);

	// ���� ���������� �Ϻθ� �׸���
	BitBlt(hdc, 0, animationOffset, BOARD_SIZE * CELL_SIZE, animationOffset, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hdc);

	// �ִϸ��̼��� �Ϸ�Ǿ��� �� ó��
	if (animationOffset >= BOARD_SIZE * CELL_SIZE)
	{
		// ���� �������� ������Ʈ
		currentStage++;
		// ��ֹ� �ʱ�ȭ
		obstacles = InitObstalce(obstacles);
		// ������ �ʱ�ȭ
		for (int i = 0; i < item_num.num; ++i) {
			item[i] = {};
			item_num = {};
		}

		Dframe = 0;
		animationOffset = 0;  // �ִϸ��̼� ������ �ʱ�ȭ
		is_first = true;

		// �ִϸ��̼��� ���� Ÿ�̸� ����
		KillTimer(hWnd, STAGE_ANIMATION);

		if (currentStage != 5) {
			// ���� ���� Ÿ�̸� ����
			SetTimer(hWnd, STAGE_START, 1000, NULL);

		}
		// ���� ���� Ÿ�̸� ����
		SetTimer(hWnd, MONSTER_PRODUSE, 3000, NULL);
		// ���� �̵� Ÿ�̸� ����
		SetTimer(hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));

		// ȭ�� ����
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void BuyItem(PLAYER* player, TCHAR* itemname, int itemprice) {

	if (player->coin >= itemprice) {

		short_ch->stop();
		ssystem->playSound(item_get_sound, 0, false, &short_ch);
		// ���� ����
		player->coin -= itemprice;

		// �Ź� �������� ��� �̵��ӵ� ����
		if (_tcscmp(itemname, L"�Ź�") == 0) {
			player->speed += 2;
		}
		// �Ѿ� �������� ��� ���ݷ� ����
		else if (_tcscmp(itemname, L"�Ѿ�") == 0) {
			player->ATP += 5;
		}
		// �� �������� ��� ���� �ӵ� ����
		else if (_tcscmp(itemname, L"��") == 0) {
			bullet_speed += 5;
		}

		// ������ ���� ���� ������Ʈ
		_tcscmp(player->shopt_item.name, itemname);
		player->shopt_item.price = itemprice;
	}
}

void GetShopItem(int Px, int Py, SHOPITEM shopitem)
{
	RECT player_rt = { Px, Py, Px + CELL_SIZE, Py + CELL_SIZE };
	RECT shoe_rt = { 110, 160, 180, 230 };
	RECT slug_rt = { 200, 160, 270, 230 };
	RECT gun_rt = { 290, 160, 360, 230 };
	RECT dest;
	// ������, �÷��̾� �浹üũ
	bool buyshoe = IntersectRect(&dest, &player_rt, &shoe_rt);
	bool buyslug = IntersectRect(&dest, &player_rt, &slug_rt);
	bool buygun = IntersectRect(&dest, &player_rt, &gun_rt);

	// �÷��̾ �Ź� ��ǥ�� ��ġ�� ��
	if (buyshoe)
	{
		BuyItem(&player, itemList[0].name, itemList[0].price);
	}
	else if (buyslug)
	{
		BuyItem(&player, itemList[1].name, itemList[1].price);
	}
	else if (buygun)
	{
		BuyItem(&player, itemList[2].name, itemList[2].price);
	}

}

void InitMerchant()
{
	merchant.p.x = 230, merchant.p.y = 30;
	merchant.current_frame = 0;

	isExit = false;
}

void CALLBACK MerchantWalking(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{

	if (isEntering)
	{
		if (merchant.p.y < 120)
		{
			if (merchant.current_frame < 2) {
				merchant.p.y += 10;
				merchant.current_frame = (merchant.frame.Dframe) % MERCHANT_SPRITE_CNT + MERCHANT_SPRITE_CNT;
				merchant.frame.Dframe++;
			}
			else {
				merchant.p.y += 10;
				merchant.frame.Dframe = 0;
			}
		}

		if (merchant.p.y == 120)
		{
			KillTimer(hWnd, MERCHANT_WALKING);
			isEntering = false;
			isShopOpen = true;
		}
	}

	if (isExit)
	{
		isPurchaseComplete = false;
		InitMerchant();
	}
}

void ShopUI(HDC memdc)
{
	if (isShopOpen) {

		// ���� ������ ǥ��
		int xShop = 150;
		int yShop = 200;

		int fontSize = 15;

		HFONT hFont = CreateFontA(-fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

		HFONT hOldFont = (HFONT)SelectObject(memdc, hFont);

		for (int i = 0; i < 3; i++)
		{
			SetBkMode(memdc, TRANSPARENT); // ����� �����ϰ� ����
			// ������ ���� ���
			TextOut(memdc, xShop - 40, yShop, itemList[i].name, lstrlen(itemList[i].name));
			TextOut(memdc, xShop - 13, yShop, L"����:", 5);
			TCHAR pricenum[20] = {};
			wsprintf(pricenum, L"%d", itemList[i].price);
			TextOut(memdc, xShop + 20, yShop, pricenum, lstrlen(pricenum));
			xShop += 90;        // ���� ������ ��ġ ����
		}

		// ������ ���� �Ϸ� �� ���� ����
		if (isPurchaseComplete)
		{
			merchant.current_frame = 2;
			isExit = true;
			isEntering = false;
			isShopOpen = false;
			isPurchaseComplete = false;
			SetTimer(_hWnd, MERCHANT_WALKING, 100, reinterpret_cast<TIMERPROC>(MerchantWalking));
		}
	}
}

// �÷��̾�
void InitPlayer(HWND hWnd)
{
	player = {};

	// �÷��̾� ���� ��Ʈ ����
	frame_numW = 1;
	frame_numL = 9;

	// �Ѿ� �̵� Ÿ�̸�
	SetTimer(hWnd, BULLET_MOVE, 100, reinterpret_cast<TIMERPROC>(BulletMove));

	// �÷��̾� �ʱ� ��ġ ����
	player.P.x = CELL_SIZE * BOARD_SIZE / 2;
	player.P.y = CELL_SIZE * BOARD_SIZE / 2;

	// �÷��̾� �ʱ� ���� ����
	player.ATP = 5;
	player.speed = 1;
	player.life = 5;
	player.coin = 15;
	player.item = false;
	player.bullet = false;
}

bool PlayerBlockObject()
{
	RECT dest;
	RECT player_rt = GetStageRect(player.P.x, player.P.y);
	RECT map_out_rt = {};

	// ��ֹ� �浹 üũ
	if (obstacles) {
		for (int i = 0; i < obstacle_num; ++i) {
			RECT obstacle_rt = GetStageRect(obstacles[i].p.x, obstacles[i].p.y);
			if (obstacles[i].kind != MOVE_ID) {
				if (IntersectRect(&dest, &player_rt, &obstacle_rt)) {
					return true;
				}
			}
			else {
				// �������� �ִϸ��̼� �۵� �� �浹 X
				if (animationOffset == 0) {
					if (IntersectRect(&dest, &player_rt, &obstacle_rt)) {
						player.life--;
						if (player.life == 0) {
							// ���� ����ó��
							PostQuitMessage(0);
						}
						while (1) {
							MONSTER m = {};
							player.P = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
							// ��ǥ�� ��ֹ��̸� �ٽ� ����
							if (!MonsterConflict(player.P, m)) {
								break;
							}
						}
						return true;
					}
				}
			}
		}
	}

	// �÷��̾� ���� �浹üũ
	if (monster_head) {
		RECT player_rt = GetStageRect(player.P.x, player.P.y);
		RECT dest;
		MONSTERNODE* p = monster_head;
		while (p->next != NULL) {
			RECT m_rt = GetStageRect(p->data.P.x, p->data.P.y);
			if (IntersectRect(&dest, &player_rt, &m_rt)) {
				short_ch->stop();
				ssystem->playSound(hit_sound, 0, false, &short_ch);
				player.life--;
				if (player.life == 0) {
					// ���� ����ó��
					PostQuitMessage(0);
				}
				if (currentStage != 5) {
					while (1) {
						MONSTER m = {};
						player.P = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
						// ��ǥ�� ��ֹ��̸� �ٽ� ����
						if (!MonsterConflict(player.P, m)) {
							break;
						}
					}
				}
				else {
					while (1) {
						MONSTER m = {};
						player.P = { rand() % ((BOARD_SIZE - 1) * CELL_SIZE), rand() % (5 * CELL_SIZE) };
						// ��ǥ�� ��ֹ��̸� �ٽ� ����
						if (!MonsterConflict(player.P, m)) {
							break;
						}
					}
				}
				return true;
			}
			p = p->next;
		}
		RECT m_rt = GetStageRect(p->data.P.x, p->data.P.y);
		if (IntersectRect(&dest, &player_rt, &m_rt)) {
			short_ch->stop();
			ssystem->playSound(hit_sound, 0, false, &short_ch);
			player.life--;
			if (player.life == 0) {
				PostQuitMessage(0);

			}
			if (currentStage != 5) {

				while (1) {
					MONSTER m = {};
					player.P = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
					// ��ǥ�� ��ֹ��̸� �ٽ� ����
					if (!MonsterConflict(player.P, m)) {
						break;
					}
				}
			}
			else {
				while (1) {
					MONSTER m = {};
					player.P = { rand() % ((BOARD_SIZE - 1) * CELL_SIZE), rand() % (5 * CELL_SIZE) };
					// ��ǥ�� ��ֹ��̸� �ٽ� ����
					if (!MonsterConflict(player.P, m)) {
						break;
					}
				}
			}
			return true;
		}
	}

	// �� �� �浹 üũ
	for (int i = 0; i <= 16; ++i) {
		// ����
		map_out_rt = GetStageRect(-1 * CELL_SIZE, i * CELL_SIZE);
		if (IntersectRect(&dest, &player_rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(BOARD_SIZE * CELL_SIZE, i * CELL_SIZE);
		if (IntersectRect(&dest, &player_rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(i * CELL_SIZE, -1 * CELL_SIZE);
		if (IntersectRect(&dest, &player_rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(i * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
		if (IntersectRect(&dest, &player_rt, &map_out_rt)) {
			if (play_time >= STAGE_TIME) {
				if (!monster_head && !respawn_head) {
					TransitionToNextStage(_hWnd);
					play_time = 0;
					return false;
				}
				return true;
			}
			else {
				return true;
			}
		}
	}

	if (currentStage == 5) {
		RECT river = { 0 * CELL_SIZE, 6 * CELL_SIZE , 16 * CELL_SIZE , 7 * CELL_SIZE };
		if (IntersectRect(&dest, &player_rt, &river)) {
			return true;
		}
	}

	return false;
}

// �Ѿ�
void CALLBACK BulletMove(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	// �Ѿ� ��ġ ����
	for (int i = 0; i < bullet_num; i++)
	{
		bullet[i].P.x += BULLET_SPEED * bullet[i].direct.x;
		bullet[i].P.y += BULLET_SPEED * bullet[i].direct.y;
	}

	// �Ѿ� ����
	DelBullet();

	//Paint ȣ��
	InvalidateRect(hWnd, NULL, false);
}

void MakeBullet(void)
{
	short_ch->stop();
	ssystem->playSound(bullet_sound, 0, false, &short_ch);
	// �Ѿ� �߰�
	bullet_num++;
	// �����Ҵ�
	if (bullet == NULL)
		bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
	else
	{
		BULLET* temp = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
		memcpy(temp, bullet, sizeof(BULLET) * bullet_num);
		free(bullet);
		bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
		memcpy(bullet, temp, sizeof(BULLET) * bullet_num);
		free(temp);
	}
}

void SetDirect(int num)
{
	// �Ѿ� ���� ����
	switch (num)
	{
	case 1:
		bullet[bullet_num - 1].P.x = player.P.x + 13;
		bullet[bullet_num - 1].P.y = player.P.y;
		bullet[bullet_num - 1].direct.x = 0;
		bullet[bullet_num - 1].direct.y = -1;
		break;
	case 2:
		bullet[bullet_num - 1].P.x = player.P.x + 30;
		bullet[bullet_num - 1].P.y = player.P.y;
		bullet[bullet_num - 1].direct.x = 1;
		bullet[bullet_num - 1].direct.y = -1;
		break;
	case 3:
		bullet[bullet_num - 1].P.x = player.P.x + 20;
		bullet[bullet_num - 1].P.y = player.P.y + 13;
		bullet[bullet_num - 1].direct.x = 1;
		bullet[bullet_num - 1].direct.y = 0;
		break;
	case 4:
		bullet[bullet_num - 1].P.x = player.P.x + 30;
		bullet[bullet_num - 1].P.y = player.P.y + 30;
		bullet[bullet_num - 1].direct.x = 1;
		bullet[bullet_num - 1].direct.y = 1;
		break;
	case 5:
		bullet[bullet_num - 1].P.x = player.P.x + 13;
		bullet[bullet_num - 1].P.y = player.P.y + 30;
		bullet[bullet_num - 1].direct.x = 0;
		bullet[bullet_num - 1].direct.y = 1;
		break;
	case 6:
		bullet[bullet_num - 1].P.x = player.P.x;
		bullet[bullet_num - 1].P.y = player.P.y + 30;
		bullet[bullet_num - 1].direct.x = -1;
		bullet[bullet_num - 1].direct.y = 1;
		break;
	case 7:
		bullet[bullet_num - 1].P.x = player.P.x;
		bullet[bullet_num - 1].P.y = player.P.y + 13;
		bullet[bullet_num - 1].direct.x = -1;
		bullet[bullet_num - 1].direct.y = 0;
		break;
	case 8:
		bullet[bullet_num - 1].P.x = player.P.x;
		bullet[bullet_num - 1].P.y = player.P.y;
		bullet[bullet_num - 1].direct.x = -1;
		bullet[bullet_num - 1].direct.y = -1;
		break;
	}
}

void DelBullet(void)
{
	for (int i = 0; i < bullet_num; i++)
	{
		if (bullet[i].P.x < CELL_SIZE * 0 || bullet[i].P.x > CELL_SIZE * BOARD_SIZE || bullet[i].P.y < CELL_SIZE * 0 || bullet[i].P.y > CELL_SIZE * BOARD_SIZE)
		{
			bullet_num--;
			for (int j = i; j < bullet_num; j++)
				bullet[j] = bullet[j + 1];
			BULLET* temp = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
			memcpy(temp, bullet, sizeof(BULLET) * bullet_num);
			free(bullet);
			bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
			memcpy(bullet, temp, sizeof(BULLET) * bullet_num);
			free(temp);
		}
		if (BlockBullet(i))
		{
			bullet_num--;
			for (int j = i; j < bullet_num; j++)
				bullet[j] = bullet[j + 1];
			BULLET* temp = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
			memcpy(temp, bullet, sizeof(BULLET) * bullet_num);
			free(bullet);
			bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
			memcpy(bullet, temp, sizeof(BULLET) * bullet_num);
			free(temp);
		}

		// �Ѿ˰� ���� �浹üũ
		if (monster_head) {
			MONSTERNODE* p = monster_head;
			RECT dest = {};
			RECT bullet_rt = {};
			RECT monster_rt = {};
			while (p->next != NULL) {
				bullet_rt = { bullet[i].P.x, bullet[i].P.y, bullet[i].P.x + 5, bullet[i].P.y + 5 };
				monster_rt = GetStageRect(p->data.P.x, p->data.P.y);
				if (IntersectRect(&dest, &bullet_rt, &monster_rt)) {
					// �Ѿ� ����
					bullet_num--;
					for (int j = i; j < bullet_num; j++)
						bullet[j] = bullet[j + 1];
					BULLET* temp = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
					memcpy(temp, bullet, sizeof(BULLET) * bullet_num);
					free(bullet);
					bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
					memcpy(bullet, temp, sizeof(BULLET) * bullet_num);
					free(temp);

					//���� HP
					p->data.HP -= player.ATP;
					if (p->data.HP <= 0) {
						if (p->data.name[0] == 'R' && !p->data.skill) {
							p->data.HP = MONSTER_HP;
							InsertMonster(&respawn_head, p->data);
						}
						else {
							// ��� ����Ʈ ��ü �߰�
							BOMB dead_data;
							InitBomb(&dead_data, p->data);
							InsertBomb(&dead_head, dead_data);
						}
						// ���� ����
						p = DelMonster(&monster_head, p->data.name);
						if (p->next == NULL) break;
					}
				}
				p = p->next;
			}

			bullet_rt = { bullet[i].P.x, bullet[i].P.y, bullet[i].P.x + 5, bullet[i].P.y + 5 };
			monster_rt = GetStageRect(p->data.P.x, p->data.P.y);
			if (IntersectRect(&dest, &bullet_rt, &monster_rt)) {
				// �Ѿ� ����
				bullet_num--;
				for (int j = i; j < bullet_num; j++)
					bullet[j] = bullet[j + 1];
				BULLET* temp = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
				memcpy(temp, bullet, sizeof(BULLET) * bullet_num);
				free(bullet);
				bullet = (BULLET*)malloc(sizeof(BULLET) * bullet_num);
				memcpy(bullet, temp, sizeof(BULLET) * bullet_num);
				free(temp);

				//���� HP
				p->data.HP -= player.ATP;
				if (p->data.HP <= 0) {
					if (p->data.name[0] == 'R' && !p->data.skill) {
						p->data.HP = MONSTER_HP;
						InsertMonster(&respawn_head, p->data);
					}
					else {
						// ��� ����Ʈ ��ü �߰�
						BOMB dead_data;
						InitBomb(&dead_data, p->data);
						InsertBomb(&dead_head, dead_data);
					}
					// ���� ����
					DelMonster(&monster_head, p->data.name);
				}
			}
		}

	
		if (currentStage == 5) {
			RECT dest = {};
			RECT bullet_rt = { bullet[i].P.x, bullet[i].P.y, bullet[i].P.x + 5, bullet[i].P.y + 5 };
			RECT mboss_rt = GetStageRect(boss.P.x, boss.P.y);
			if (IntersectRect(&dest, &bullet_rt, &mboss_rt)) {
				boss.hp -= player.ATP;
				if (clear == 0) {
					if (boss.hp <= 0) {
						short_ch->stop();
						ssystem->playSound(gameclear_sound, 0, false, &short_ch);
						TCHAR text[30] = L"���� Ŭ���� �ϼ̽��ϴ�!";
						TCHAR text_caption[30] = L"CowBoy";
						clear++;
						for (int i = 1; i <= BOSS_OBSTACLE; ++i) {
							KillTimer(_hWnd, i);
						}
						if (MessageBox(_hWnd, text, text_caption, MB_OK)) {
							PostQuitMessage(0);
						}
					}
				}
			}
		}
	}
}

bool BlockBullet(int num)
{
	RECT bullet_rt = { bullet[num].P.x, bullet[num].P.y, bullet[num].P.x + 5, bullet[num].P.y + 5 };
	RECT dest = {};

	for (int i = 0; i < obstacle_num; ++i) {
		RECT obstacle_rt = GetStageRect(obstacles[i].p.x, obstacles[i].p.y);

		if (IntersectRect(&dest, &bullet_rt, &obstacle_rt)) {
			return true;
		}
	}

	return false;
}

void ShotgunBullet(int n1, int n2, int n3)
{
	int num[3] = { n1, n2, n3 };

	// �Ѿ� ���⺰ 3�� ����
	for (int i = 0; i < 3; i++)
	{
		MakeBullet();
		SetDirect(num[i]);
	}
}

void WaterWheelBullet(void)
{
	// ��� ���� �Ѿ� ����
	for (int i = 0; i < 8; i++)
	{
		MakeBullet();
		SetDirect(i + 1);
	}
}

// ������
void CALLBACK CountItemTime(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (player.effect_item.effect) {
		// ������ ���ӽð� ����
		player.effect_item.use_cnt++;

		// ������ �ɷ� ����
		if (player.effect_item.use_cnt == 5)
		{
			// 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �ð�
			if (player.effect_item.num == 3)
				player.speed--;
			else if (player.effect_item.num == 4)
				bullet_speed += 5;
			else if (player.effect_item.num == 6 || player.effect_item.num == 7)
				player.bullet = false;
			else if (player.effect_item.num == 8)
			{
				// ���� ���� Ÿ�̸� ����
				SetTimer(hWnd, STAGE_START, 1000, NULL);
				// ���� ���� Ÿ�̸� �ٽ� ����
				
				if (play_time >= STAGE_TIME) {
					// ���� �̵� Ÿ�̸� �ٽ� ����
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
				else {
					SetTimer(_hWnd, MONSTER_PRODUSE, 3000, NULL);
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
			}
			item[item_num.num].num = 0;
			item[item_num.num].effect = false;
			item[item_num.num].use_cnt = 0;

			player.effect_item.num = 0;
			player.effect_item.effect = false;
			player.effect_item.use_cnt = 0;
			KillTimer(hWnd, ITEM_DURATION);
			KillTimer(hWnd, EFFECT_LOAD);
		}
		else if (player.effect_item.use_cnt == 1 && player.effect_item.num == 5)
		{
			if (play_time >= STAGE_TIME) {
				// ���� �̵� Ÿ�̸� �ٽ� ����
				SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
			}
			else {
				SetTimer(_hWnd, MONSTER_PRODUSE, 3000, NULL);
				SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
			}
			item[item_num.num].num = 0;
			item[item_num.num].effect = false;
			item[item_num.num].use_cnt = 0;

			player.effect_item.num = 0;
			player.effect_item.effect = false;
			player.effect_item.use_cnt = 0;
			KillTimer(hWnd, ITEM_DURATION);
			KillTimer(hWnd, EFFECT_LOAD);
		}
	}
}

ITEM GetItem(int Px, int Py, ITEM item_, int idx)
{
	RECT player_rt = { Px, Py, Px + CELL_SIZE, Py + CELL_SIZE };
	RECT item_rt = { item_.P.x, item_.P.y, item_.P.x + CELL_SIZE, item_.P.y + CELL_SIZE };
	RECT dest;
	// ������, �÷��̾� �浹üũ
	bool result = IntersectRect(&dest, &player_rt, &item_rt);

	// �÷��̰� �������� �Ծ��� ��
	if (result)
	{
		short_ch->stop();
		ssystem->playSound(item_get_sound, 0, false, &short_ch);
		// ������ ���̱�
		item_.show = false;
		// �÷��̾ ������ ���ϰ� �ְ� �ϱ�
		player.item = true;
		player.item_idx = idx;

		// 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �ð�
		// ���, ���� ���� ����
		if (item_.num == 1)
			player.life++;
		else if (item_.num == 2)
			player.coin++;
	}

	return item_;
}

void UseItem(HWND hWnd)
{
	if (player.item)
	{
		short_ch->stop();
		ssystem->playSound(item_use_sound, 0, false, &short_ch);
		// ���� ������ �ɷ� ����
		if (player.effect_item.effect) {
			// 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �ð�
			if (player.effect_item.num == 3)
				player.speed--;
			else if (player.effect_item.num == 4)
				bullet_speed += 5;
			else if (player.effect_item.num == 6 || player.effect_item.num == 7)
				player.bullet = false;
			else if (player.effect_item.num == 8)
			{
				// ���� ���� Ÿ�̸� ����
				SetTimer(hWnd, STAGE_START, 1000, NULL);
				// ���� ���� Ÿ�̸� �ٽ� ����

				if (play_time >= STAGE_TIME) {
					// ���� �̵� Ÿ�̸� �ٽ� ����
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
				else {
					SetTimer(_hWnd, MONSTER_PRODUSE, 3000, NULL);
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
			}
			item[item_num.num].num = 0;
			item[item_num.num].effect = false;
			item[item_num.num].use_cnt = 0;

			player.effect_item.num = 0;
			player.effect_item.effect = false;
			player.effect_item.use_cnt = 0;
			KillTimer(hWnd, ITEM_DURATION);
			KillTimer(hWnd, EFFECT_LOAD);
			if (player.effect_item.use_cnt == 1)
			{
				if (play_time >= STAGE_TIME) {
					// ���� �̵� Ÿ�̸� �ٽ� ����
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
				else {
					SetTimer(_hWnd, MONSTER_PRODUSE, 3000, NULL);
					SetTimer(_hWnd, MONSTER_MOVE, 60, reinterpret_cast<TIMERPROC>(MonsterMove));
				}
				item[item_num.num].num = 0;
				item[item_num.num].effect = false;
				item[item_num.num].use_cnt = 0;

				player.effect_item.num = 0;
				player.effect_item.effect = false;
				player.effect_item.use_cnt = 0;
				KillTimer(hWnd, ITEM_DURATION);
				KillTimer(hWnd, EFFECT_LOAD);
			}
		}

		// �÷��̾� ������ �Ҹ�
		player.item = false;
		//����Ʈ Ȱ��ȭ
		item[player.item_idx].effect = true;

		// �÷��̾� ȿ��
		player.effect_item = item[player.item_idx];

		// 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �ð�
		// ������ �ɷ� �ݿ�
		if (item[player.item_idx].num == 3)
			player.speed++;
		else if (item[player.item_idx].num == 4)
			bullet_speed -= 5;
		else if (item[player.item_idx].num == 5) {
			// ���� ���� Ÿ�̸� ����
			KillTimer(hWnd, MONSTER_PRODUSE);
			// ���� �̵� Ÿ�̸� ����
			KillTimer(hWnd, MONSTER_MOVE);
			if (monster_head) {
				MONSTERNODE* p = monster_head;
				while (p->next != NULL) {
					if (p->data.name[0] == 'R' && !p->data.skill) {
						p->data.HP = MONSTER_HP;
						InsertMonster(&respawn_head, p->data);
					}
					else {
						// ��� ����Ʈ ��ü �߰�
						BOMB dead_data;
						InitBomb(&dead_data, p->data);
						InsertBomb(&dead_head, dead_data);
					}
					// ���� ����
					p = DelMonster(&monster_head, p->data.name);
					if (p->next == NULL) break;
				}
				if (p->data.name[0] == 'R' && !p->data.skill) {
					p->data.HP = MONSTER_HP;
					InsertMonster(&respawn_head, p->data);
				}
				else {
					// ��� ����Ʈ ��ü �߰�
					BOMB dead_data;
					InitBomb(&dead_data, p->data);
					InsertBomb(&dead_head, dead_data);
				}
				// ���� ����
				p = DelMonster(&monster_head, p->data.name);
			}
		}
		else if (item[player.item_idx].num == 6 || item[player.item_idx].num == 7)
			player.bullet = true;
		else if (item[player.item_idx].num == 8)
		{
			// ���� ���� Ÿ�̸� ����
			KillTimer(hWnd, MONSTER_PRODUSE);
			// ���� �̵� Ÿ�̸� ����
			KillTimer(hWnd, MONSTER_MOVE);
			// ���� �ð� ����
			KillTimer(hWnd, STAGE_START);
		}
		// ����Ʈ Ÿ�̸� ����
		SetTimer(hWnd, EFFECT_LOAD, 100, reinterpret_cast<TIMERPROC>(EffectLoad));
		// ������ ���ӽð� Ÿ�̸� ����
		SetTimer(hWnd, ITEM_DURATION, 1000, reinterpret_cast<TIMERPROC>(CountItemTime));
	}
}

// ����
void initMonster(MONSTER* data, PLAYER player)
{
	(*data) = {};
	int kind = (rand() % 5) * 10;
	int rand_p = rand() % 4;
	POINT p = {};
	TCHAR id[20] = L"";

	// ������ ���� ID �Է�
	switch (kind) {
	case NORMAL_MONSTER:
		_tcscpy(data->name, L"N");
		wsprintf(id, L"%d", normal_id++);
		_tcscat(data->name, id);
		data->HP = MONSTER_HP;
		data->target = player.P;
		break;
	case RESPAWN_MONSTER:
		_tcscpy(data->name, L"R");
		wsprintf(id, L"%d", respawn_id++);
		_tcscat(data->name, id);
		data->HP = MONSTER_HP;
		data->target = player.P;
		break;
	case TANK_MONSTER:
		_tcscpy(data->name, L"T");
		wsprintf(id, L"%d", tank_id++);
		_tcscat(data->name, id);
		data->HP = MONSTER_HP * 5;
		data->target = player.P;
		break;
	case OBSTACLE_MONSTER:
		_tcscpy(data->name, L"O");
		wsprintf(id, L"%d", onstacle_id++);
		_tcscat(data->name, id);
		data->HP = MONSTER_HP;
		while (1) {
			data->target = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
			// ��ǥ�� ��ֹ��̸� �ٽ� ����
			if (!MonsterConflict(data->target, *data)) {
				break;
			}
		}
		break;
	case BOOMER_MONSTER:
		_tcscpy(data->name, L"B");
		wsprintf(id, L"%d", boomer_id++);
		_tcscat(data->name, id);
		data->HP = MONSTER_HP;
		while (1) {
			data->target = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
			// ��ǥ�� ��ֹ��̸� �ٽ� ����
			if (!MonsterConflict(data->target, *data)) {
				break;
			}
		}
		break;
	default:
		break;
	}

	// ��ġ ���� ����
	switch (rand_p) {
	case 0:
		// ����
		p = { (rand() % 3) + 7, 0};
		break;
	case 1:
		// ����
		p = { BOARD_SIZE - 1, (rand() % 3) + 7 };
		break;
	case 2:
		// ����
		p = { (rand() % 3) + 7, BOARD_SIZE - 1 };
		break;
	case 3:
		// ����
		p = { 0, (rand() % 3) + 7 };
		break;
	default:
		break;
	}

	data->P = { p.x * CELL_SIZE, p.y * CELL_SIZE };
}

// ���� ���
// ============================================ //
MONSTERNODE* Gettail(MONSTERNODE* head)
{
	MONSTERNODE* p = head;
	while (p->next != NULL) {
		p = p->next;
	}

	return p;
}

void InsertMonster(MONSTERNODE** head, MONSTER data)
{
	if (*head == NULL) {
		MONSTERNODE* _new = (MONSTERNODE*)malloc(sizeof(MONSTERNODE));

		_new->data = data;
		_new->next = NULL;

		*head = _new;
	}

	else {
		MONSTERNODE* _new = (MONSTERNODE*)malloc(sizeof(MONSTERNODE));

		_new->data = data;
		_new->next = NULL;

		Gettail(*head)->next = _new;
	}
}

MONSTERNODE* SearchNode(MONSTERNODE* head, MONSTERNODE* node, TCHAR* name)
{
	if (node == NULL) {
		return NULL;
	}

	if (_tcscmp(name, node->data.name) == 0) {
		return node;
	}
	else if (_tcscmp(name, node->next->data.name) == 0) {
		return node;
	}

	SearchNode(head, node->next, name);
}

MONSTERNODE* DelMonster(MONSTERNODE** head, TCHAR* name)
{
	MONSTERNODE* temp = NULL;
	temp = SearchNode(*head, *head, name);

	short_ch->stop();
	ssystem->playSound(hit_sound, 0, false, &short_ch);

	if (temp == NULL) return NULL;

	if (temp == *head && _tcscmp(name, (*head)->data.name) == 0) {
		*head = temp->next;
		free(temp);
		return *head;
	}
	else {
		MONSTERNODE* temp_next = temp->next;

		temp->next = temp_next->next;
		free(temp_next);
		return temp;
	}
}
// ============================================ //

// ��ź ���
// ============================================ //
void InitBomb(BOMB* bombdata, MONSTER monster)
{
	(*bombdata) = {};
	bombdata->P = monster.P;
}

void InitBombing(BOMB* bombing, BOMB bombdata)
{
	(*bombing) = bombdata;
	bombing->cnt--;
	bombing->P = { bombing->P.x , bombing->P.y - 1 * CELL_SIZE };
}

BOMBNODE* Bomb_Gettail(BOMBNODE* head)
{
	BOMBNODE* p = head;
	while (p->next != NULL) {
		p = p->next;
	}

	return p;
}

void InsertBomb(BOMBNODE** head, BOMB data)
{
	if (*head == NULL) {
		BOMBNODE* _new = (BOMBNODE*)malloc(sizeof(BOMBNODE));

		_new->data = data;
		_new->next = NULL;

		*head = _new;
	}
	else {
		BOMBNODE* _new = (BOMBNODE*)malloc(sizeof(BOMBNODE));

		_new->data = data;
		_new->next = NULL;

		Bomb_Gettail(*head)->next = _new;
	}
}

BOMBNODE* Bomb_SearchNode(BOMBNODE* head, BOMBNODE* node, int frame)
{
	if (node == NULL) {
		return NULL;
	}

	if (node->data.current_frame == frame) {
		return node;
	}
	else if (node->next->data.current_frame == frame) {
		return node;
	}

	node = node->next;
	Bomb_SearchNode(head, node, frame);
}

BOMBNODE* DelBomb(BOMBNODE** head, int frame)
{
	BOMBNODE* temp = NULL;
	temp = Bomb_SearchNode(*head, *head, frame);

	if (temp == NULL) return NULL;

	if (temp == *head && (*head)->data.current_frame == frame) {
		*head = temp->next;
		free(temp);
		return *head;
	}
	else {
		BOMBNODE* temp_next = temp->next;

		temp->next = temp_next->next;
		free(temp_next);
		return temp;
	}
}
// ============================================ //

void OneMonsterMove(MONSTERNODE** node, POINT target)
{
	int dx = target.x - (*node)->data.P.x;
	int dy = target.y - (*node)->data.P.y;
	(*node)->data.direct.x = dx;
	(*node)->data.direct.y = dy;

	double distance = sqrt(dx * dx + dy * dy);
	if (distance <= MONSTER_SPPED) {
		// ��ǥ ��ġ�� �������� ���
		(*node)->data.P = target;
	}
	else {
		// ������ �ӵ��� �̵�
		double ratio = MONSTER_SPPED / distance;
		(*node)->data.P.x += static_cast<int>(dx * ratio);
		(*node)->data.P.y += static_cast<int>(dy * ratio);
		if (MonsterConflict((*node)->data.P, (*node)->data)) {
			(*node)->data.P.x -= static_cast<int>(dx * ratio);
			(*node)->data.P.y -= static_cast<int>(dy * ratio);
			while (1) {
				(*node)->data.target = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
				// ��ǥ�� ��ֹ��̸� �ٽ� ����
				if (!MonsterConflict((*node)->data.target, (*node)->data)) {
					break;
				}
			}
		}
	}

	if ((*node)->data.direct.x <= 0) {
		(*node)->data.frame.Lframe++;
		(*node)->data.current_frame = (*node)->data.frame.Lframe % MONSTER_SPRITE_WCNT + MONSTER_SPRITE_WCNT * 3;
	}
	else if ((*node)->data.direct.x > 0) {
		(*node)->data.frame.Rframe++;
		(*node)->data.current_frame = (*node)->data.frame.Rframe % MONSTER_SPRITE_WCNT + MONSTER_SPRITE_WCNT * 1;
	}
	if ((*node)->data.direct.y <= 0) {
		(*node)->data.frame.Uframe++;
		(*node)->data.current_frame = (*node)->data.frame.Uframe % MONSTER_SPRITE_WCNT + MONSTER_SPRITE_WCNT * 0;
	}
	else if ((*node)->data.direct.y > 0) {
		(*node)->data.frame.Dframe++;
		(*node)->data.current_frame = (*node)->data.frame.Dframe % MONSTER_SPRITE_WCNT + MONSTER_SPRITE_WCNT * 2;
	}
}

void CALLBACK MonsterMove(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	// ����
	if (monster_head) {
		MONSTERNODE* p = monster_head;
		while (p->next != NULL) {
			// �Ϲ�, ��ġ, ��Ȱ ����
			if (p->data.name[0] == 'N' || p->data.name[0] == 'R' || p->data.name[0] == 'T') {
				OneMonsterMove(&p, player.P);
			}
			// �� �� ����
			else {
				// ��ź ����
				if (p->data.name[0] == 'B') {
					// ��ǥ���� �������� ��(��ź ��ġ ��)
					if ((p->data.P.x == p->data.target.x && p->data.P.y == p->data.target.y) || p->data.skill) {
						if(!p->data.skill) p->data.skill = true;
						MonsterSkillCnt(&p->data);
					}
					// ��� �̵�
					else {
						OneMonsterMove(&p, p->data.target);
					}
				}
				// ��ֹ� ����
				else if (p->data.name[0] == 'O') {
					// ��ǥ���� �������� ��
					if ((p->data.P.x == p->data.target.x && p->data.P.y == p->data.target.y) || p->data.skill) {
						if (!p->data.skill) {
							p->data.skill = true;
							// ��ֹ��� �Ǿ��� �� HP�� �� ��
							p->data.HP *= 2;
						}
					}
					// ��� �̵�
					else {
						OneMonsterMove(&p, p->data.target);
					}
				}
			}
			p = p->next;
		}

		// �Ϲ�, ��ġ, ��Ȱ ����
		if (p->data.name[0] == 'N' || p->data.name[0] == 'R' || p->data.name[0] == 'T') {
			OneMonsterMove(&p, player.P);
		}
		// �� �� ����
		else {
			// ��ź ����
			if (p->data.name[0] == 'B') {
				// ��ǥ���� �������� ��(��ź ��ġ ��)
				if ((p->data.P.x == p->data.target.x && p->data.P.y == p->data.target.y) || p->data.skill) {
					if (!p->data.skill) p->data.skill = true;
					MonsterSkillCnt(&p->data);
				}
				// ��� �̵�
				else {
					OneMonsterMove(&p, p->data.target);
				}
			}
			// ��ֹ� ����
			else if (p->data.name[0] == 'O') {
				// ��ǥ���� �������� ��
				if ((p->data.P.x == p->data.target.x && p->data.P.y == p->data.target.y) || p->data.skill) {
					if (!p->data.skill) {
						p->data.skill = true;
						// ��ֹ��� �Ǿ��� �� HP�� �� ��
						p->data.HP *= 3;
					}
				}
				// ��� �̵�
				else {
					OneMonsterMove(&p, p->data.target);
				}
			}
		}
	}

	// ��Ȱ ����
	if (respawn_head) {
		MONSTERNODE* p = respawn_head;
		while (p->next != NULL) {
			MonsterSkillCnt(&p->data);
			p = p->next;
		}
		MonsterSkillCnt(&p->data);
	}
	
	// ��ź
	if (bomb_head) {
		BOMBNODE* p = bomb_head;
		while (p->next != NULL) {
			// ��ź �ð� ���
			p->data.cnt++;
			// ������ ����
			if (p->data.cnt % (BOMB_TIME / BOMB_SPRITE_CNT) == 0) p->data.current_frame++;
			// ���� ������ �������̸� ��Ʈ����
			if (p->data.current_frame == BOMB_SPRITE_CNT) {
				BOMB bombing_data;
				InitBombing(&bombing_data, p->data);
				InsertBomb(&bombing_head, bombing_data);
				p = DelBomb(&bomb_head, BOMB_SPRITE_CNT);
				if (p->next == NULL) break;
			}
			p = p->next;
		}
		p->data.cnt++;
		if (p->data.cnt % (BOMB_TIME / BOMB_SPRITE_CNT) == 0) p->data.current_frame++;
		if (p->data.current_frame == BOMB_SPRITE_CNT) {
			boom_ch->stop();
			ssystem->playSound(boom_sound, 0, false, &boom_ch);
			boom_ch->setVolume(0.3);
			BOMB bombing_data;
			InitBombing(&bombing_data, p->data);
			InsertBomb(&bombing_head, bombing_data);
			DelBomb(&bomb_head, BOMB_SPRITE_CNT);
		}
	}

	// ����
	if (bombing_head) {
		BOMBNODE* p = bombing_head;
		while (p->next != NULL) {
			// ���� �ð� �� ������
			p->data.cnt++;
			if (p->data.cnt % (BOMBING_SPRITE_CNT * 2) == 0) p->data.current_frame++;
			if (p->data.current_frame == BOMBING_SPRITE_CNT) {
				p = DelBomb(&bombing_head, BOMBING_SPRITE_CNT);
				if (p->next == NULL) break;
			}

			BombingDamage(p->data);

			p = p->next;
		}
		p->data.cnt++;
		if (p->data.cnt % (BOMBING_SPRITE_CNT) == 0) p->data.current_frame++;
		if (p->data.current_frame == BOMBING_SPRITE_CNT) DelBomb(&bombing_head, BOMBING_SPRITE_CNT);
		BombingDamage(p->data);
	}
	
	// ��� ����Ʈ
	if (dead_head) {
		BOMBNODE* p = dead_head;
		while (p->next != NULL) {
			p->data.cnt++;
			if (p->data.cnt % (DEAD_SPRITE_CNT / 6) == 0) p->data.current_frame++;
			if (p->data.current_frame == DEAD_SPRITE_CNT) {
				// ���������� ������ ���
				int rand_num = rand() % 2;
				if(rand_num)
					MakeItem(_hWnd, p->data.P);

				// ��� ����Ʈ ����
				p = DelBomb(&dead_head, DEAD_SPRITE_CNT);
				if(p->next == NULL) break;
			}
			p = p->next;
		}
		p->data.cnt++;
		if (p->data.cnt % (DEAD_SPRITE_CNT / 6) == 0) p->data.current_frame++;
		if (p->data.current_frame == DEAD_SPRITE_CNT) {
			// ���������� ������ ���
			int rand_num = rand() % 2;
			if (rand_num)
				MakeItem(_hWnd, p->data.P);

			DelBomb(&dead_head, DEAD_SPRITE_CNT);
		}
	}


	// ��ֹ� ������
	if (obstacles) {
		for (int i = 0; i < obstacle_num; ++i) {
			if (obstacles[i].kind == MOVE_ID) {
				obstacles[i].p.x += obstacles[i].direct.x * obstacles[i].speed;
				obstacles[i].p.y += obstacles[i].direct.y * obstacles[i].speed;

				obstacles[i].current_frame++;
				if (obstacles[i].direct.x > 0) {
					obstacles[i].current_frame = (obstacles[i].current_frame % OBSTACLE_SPRITE_WCNT) + (OBSTACLE_SPRITE_WCNT * 1);
				}
				else if (obstacles[i].direct.x < 0) {
					obstacles[i].current_frame = (obstacles[i].current_frame % OBSTACLE_SPRITE_WCNT) + (OBSTACLE_SPRITE_WCNT * 3);
				}
				if (obstacles[i].direct.y > 0) {
					obstacles[i].current_frame = (obstacles[i].current_frame % OBSTACLE_SPRITE_WCNT) + (OBSTACLE_SPRITE_WCNT * 2);
				}
				else if (obstacles[i].direct.y < 0) {
					obstacles[i].current_frame = (obstacles[i].current_frame % OBSTACLE_SPRITE_WCNT) + (OBSTACLE_SPRITE_WCNT * 0);
				}
				// �ݴ������� �����ϸ� �ٽ� �ݴ��
				if (obstacles[i].p.x == abs((BOARD_SIZE * CELL_SIZE ) - obstacles[i].start_p.x) || obstacles[i].p.y == abs((BOARD_SIZE * CELL_SIZE) - obstacles[i].start_p.y)) {
					obstacles[i].direct.x *= -1;
					obstacles[i].direct.y *= -1;
				}
				else if (obstacles[i].p.x == obstacles[i].start_p.x && obstacles[i].p.y == obstacles[i].start_p.y) {
					obstacles[i].direct.x *= -1;
					obstacles[i].direct.y *= -1;
				}
			}
		}
	}

	InvalidateRect(hWnd, NULL, false);
}

bool MonsterConflict(POINT p, MONSTER monster)
{
	RECT rt = GetStageRect(p.x, p.y);
	RECT dest = {};
	RECT map_out_rt = {};

	// ��ֹ� �浹 üũ
	if (obstacles) {
		for (int i = 0; i < obstacle_num; ++i) {
			if (obstacles[i].kind != MOVE_ID) {
				RECT obstacle_rt = GetStageRect(obstacles[i].p.x, obstacles[i].p.y);

				if (IntersectRect(&dest, &rt, &obstacle_rt)) {
					return true;
				}
			}
		}
	}

	// �ٸ� ���Ϳ��� �浹 üũ
	if (monster_head) {
		MONSTERNODE* q = monster_head;
		while (q->next != NULL) {
			if (_tcscmp(monster.name, q->data.name) != 0) {
				RECT obstacle_rt = GetStageRect(q->data.P.x, q->data.P.y);
				if (IntersectRect(&dest, &rt, &obstacle_rt)) {
					return true;
				}
			}
			q = q->next;
		}
		if (_tcscmp(monster.name, q->data.name) != 0) {
			RECT obstacle_rt = GetStageRect(q->data.P.x, q->data.P.y);
			if (IntersectRect(&dest, &rt, &obstacle_rt)) {
				return true;
			}
		}
	}

	if (respawn_head) {
		MONSTERNODE* q = respawn_head;
		while (q->next != NULL) {
			if (_tcscmp(monster.name, q->data.name) != 0) {
				RECT respawn_rt = GetStageRect(q->data.P.x, q->data.P.y);
				if (IntersectRect(&dest, &rt, &respawn_rt)) {
					return true;
				}
			}
			q = q->next;
		}
		if (_tcscmp(monster.name, q->data.name) != 0) {
			RECT respawn_rt = GetStageRect(q->data.P.x, q->data.P.y);
			if (IntersectRect(&dest, &rt, &respawn_rt)) {
				return true;
			}
		}
	}

	// �� �� �浹 üũ
	for (int i = 7; i <= 9; ++i) {
		// ����
		map_out_rt = GetStageRect(-1 * CELL_SIZE, i * CELL_SIZE);
		if (IntersectRect(&dest, &rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(BOARD_SIZE * CELL_SIZE, i * CELL_SIZE);
		if (IntersectRect(&dest, &rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(i * CELL_SIZE, -1 * CELL_SIZE);
		if (IntersectRect(&dest, &rt, &map_out_rt)) {
			return true;
		}
		// ����
		map_out_rt = GetStageRect(i * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
		if (IntersectRect(&dest, &rt, &map_out_rt)) {
			return true;
		}
	}

	return false;
}

void MonsterSkillCnt(MONSTER* data)
{
	data->time_cnt++;
	if (data->time_cnt == 40) {
		data->time_cnt = 0;
		// ��ź ����
		if (data->name[0] == 'B') {
			data->skill = false;
			BOMB bomb = {};
			InitBomb(&bomb, *data);
			InsertBomb(&bomb_head, bomb);
		}
		if (data->name[0] == 'R') {
			// �ɷ��� Ȱ��ȭ�ϰ� ���� ���� �̵�
			data->skill = true;
			InsertMonster(&monster_head, *data);
			DelMonster(&respawn_head, data->name);
			return;
		}

		while (1) {
			data->target = { rand() % ((BOARD_SIZE - 2)* CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
			// ��ǥ�� ��ֹ��̸� �ٽ� ����
			if (!MonsterConflict(data->target, *data)) {
				break;
			}
		}
	}
}

OBSTACLE* InitObstalce(OBSTACLE* obstacles)
{
	obstacle_num = 0;

	switch (currentStage) {
	case 1:
		// �̹� �迭�� ������ ���� �� ���Ҵ�
		if ((obstacles)) {
			free(obstacles);
		}

		// ��ֹ��� ������ �ʱ� ������ �迭�� ����
		(obstacles) = (OBSTACLE*)malloc(sizeof(OBSTACLE) * (WALL_OBSTACLE_CNT + 2));
		// ����
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, 0 };
		}
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, (BOARD_SIZE - 1) * CELL_SIZE };
		}
		// ����
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { 0, i * CELL_SIZE };
		}
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { (BOARD_SIZE - 1) * CELL_SIZE, i * CELL_SIZE };

		}

		for (int i = 0; i < 2; ++i) {
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = MOVE_ID;
			obstacles[obstacle_num].speed = rand() % OBSTACLE_SPEED + 1;
			if (i == 0) {
				obstacles[obstacle_num].direct = { 1, 0 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT;
				obstacles[obstacle_num].p = { 3 * CELL_SIZE,  3 * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}
			else {
				obstacles[obstacle_num].direct = { -1, 0 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT * 3;
				obstacles[obstacle_num].p = { (BOARD_SIZE - 3) * CELL_SIZE,  (BOARD_SIZE - 3) * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}

		}
		break;
	case 2:
		// �̹� �迭�� ������ ���� �� ���Ҵ�
		if ((obstacles)) {
			free(obstacles);
		}

		(obstacles) = (OBSTACLE*)malloc(sizeof(OBSTACLE) * (WALL_OBSTACLE_CNT + 12));
		// ����
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, 0 };
		}
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, (BOARD_SIZE - 1) * CELL_SIZE };
		}
		// ����
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { 0, i * CELL_SIZE };
		}
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { (BOARD_SIZE - 1) * CELL_SIZE, i * CELL_SIZE };

		}

		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 4 * CELL_SIZE, 4 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 5 * CELL_SIZE, 4 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 4 * CELL_SIZE, 5 * CELL_SIZE };

		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 10 * CELL_SIZE, 4 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 11 * CELL_SIZE, 4 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 11 * CELL_SIZE, 5 * CELL_SIZE };

		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 4 * CELL_SIZE, 10 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 4 * CELL_SIZE, 11 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 5 * CELL_SIZE, 11 * CELL_SIZE };

		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 11 * CELL_SIZE, 10 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 11 * CELL_SIZE, 11 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 10 * CELL_SIZE, 11 * CELL_SIZE };
		break;
	case 3:
		if ((obstacles)) {
			free(obstacles);
		}

		(obstacles) = (OBSTACLE*)malloc(sizeof(OBSTACLE) * (WALL_OBSTACLE_CNT + 4));
		// ����
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, 0 };
		}
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, (BOARD_SIZE - 1) * CELL_SIZE };
		}
		// ����
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { 0, i * CELL_SIZE };
		}
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { (BOARD_SIZE - 1) * CELL_SIZE, i * CELL_SIZE };
		}

		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 8 * CELL_SIZE, 4 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 4 * CELL_SIZE, 8 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 8 * CELL_SIZE, 12 * CELL_SIZE };
		obstacles[obstacle_num] = {};
		obstacles[obstacle_num].kind = WOOD_ID;
		obstacles[obstacle_num++].p = { 12 * CELL_SIZE, 8 * CELL_SIZE };
		break;
	case 4:
		if ((obstacles)) {
			free(obstacles);
		}

		(obstacles) = (OBSTACLE*)malloc(sizeof(OBSTACLE) * (WALL_OBSTACLE_CNT + 4));
		// ����
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, 0 };
		}
		for (int i = 0; i < 16; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { i * CELL_SIZE, (BOARD_SIZE - 1) * CELL_SIZE };
		}
		// ����
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { 0, i * CELL_SIZE };
		}
		for (int i = 1; i < 15; ++i) {
			if (i >= 7 && i <= 9) continue;
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = WALL_ID;
			obstacles[obstacle_num++].p = { (BOARD_SIZE - 1) * CELL_SIZE, i * CELL_SIZE };

		}

		for (int i = 0; i < 4; ++i) {
			obstacles[obstacle_num] = {};
			obstacles[obstacle_num].kind = MOVE_ID;
			obstacles[obstacle_num].speed = rand() % OBSTACLE_SPEED + 1;
			if (i == 0) {
				obstacles[obstacle_num].direct = { 1, 0 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT;
				obstacles[obstacle_num].p = { 3 * CELL_SIZE,  3 * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}
			else if(i == 1){
				obstacles[obstacle_num].direct = { -1, 0 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT * 3;
				obstacles[obstacle_num].p = { (BOARD_SIZE - 3) * CELL_SIZE,  (BOARD_SIZE - 3) * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}
			else if (i == 2) {
				obstacles[obstacle_num].direct = { 1, 1 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT * 0;
				obstacles[obstacle_num].p = { 3 * CELL_SIZE,  3 * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}
			else if (i == 3) {
				obstacles[obstacle_num].direct = { 1, -1 };
				obstacles[obstacle_num].current_frame = OBSTACLE_SPRITE_WCNT;
				obstacles[obstacle_num].p = { 3 * CELL_SIZE,  (BOARD_SIZE - 3) * CELL_SIZE };
				obstacles[obstacle_num].start_p = obstacles[obstacle_num].p;
				obstacle_num++;
			}
		}
		break;
	case 5:
		if ((obstacles)) {
			free(obstacles);
		}
		obstacles = NULL;
		break;
	default:
		break;
	}

	return obstacles;
}

void KickBomb()
{
	RECT player_rt = GetStageRect(player.P.x, player.P.y);
	RECT dest;
	RECT b_rt = {};

	if (bomb_head) {
		short_ch->stop();
		ssystem->playSound(bullet_sound, 0, false, &short_ch);
		BOMBNODE* p = bomb_head;
		while (p->next != NULL) {
			POINT change_p = {};
			b_rt = GetStageRect(p->data.P.x, p->data.P.y);
			if (IntersectRect(&dest, &player_rt, &b_rt)) {
				switch (frame_numL)	{
				case 0:
					change_p.x += CELL_SIZE;
					break;
				case 1:
					change_p.x -= CELL_SIZE;
					break;
				case 2:
					change_p.x += CELL_SIZE;
					change_p.y += CELL_SIZE;
					break;
				case 4:
					change_p.x += CELL_SIZE;
					change_p.y -= CELL_SIZE;
					break;
				case 5:
					change_p.y -= CELL_SIZE;
					break;
				case 6:
					change_p.x -= CELL_SIZE;
					change_p.y -= CELL_SIZE;
					break;
				case 7:
					change_p.x -= CELL_SIZE;
					break;
				case 8:
					change_p.x -= CELL_SIZE;
					change_p.y += CELL_SIZE;
					break;
				case 9:
					change_p.y += CELL_SIZE;
					break;
				default:
					break;
				}

				// ������Ʈ�� ���� ������ �̵�
				while (1) {
					MONSTER m = {};
					p->data.P.x += change_p.x;
					p->data.P.y += change_p.y;
					if (MonsterConflict(p->data.P, m)) {
						return;
					}
				}
			}
			p = p->next;
		}
		POINT change_p = {};
		b_rt = GetStageRect(p->data.P.x, p->data.P.y);
		if (IntersectRect(&dest, &player_rt, &b_rt)) {
			switch (frame_numL) {
			case 0:
				change_p.x += CELL_SIZE;
				break;
			case 1:
				change_p.x -= CELL_SIZE;
				break;
			case 2:
				change_p.x += CELL_SIZE;
				change_p.y += CELL_SIZE;
				break;
			case 4:
				change_p.x += CELL_SIZE;
				change_p.y -= CELL_SIZE;
				break;
			case 5:
				change_p.y -= CELL_SIZE;
				break;
			case 6:
				change_p.x -= CELL_SIZE;
				change_p.y -= CELL_SIZE;
				break;
			case 7:
				change_p.x -= CELL_SIZE;
				break;
			case 8:
				change_p.x -= CELL_SIZE;
				change_p.y += CELL_SIZE;
				break;
			case 9:
				change_p.y += CELL_SIZE;
				break;
			default:
				break;
			}
			while (1) {
				MONSTER m = {};
				p->data.P.x += change_p.x;
				p->data.P.y += change_p.y;
				if (MonsterConflict(p->data.P, m)) {
					return;
				}
			}
		}
	}
}

void BombingDamage(BOMB bombing)
{
	RECT dest;
	RECT b_rt = { bombing.P.x, bombing.P.y, bombing.P.x + CELL_SIZE * BOMBING_SIZE, bombing.P.y + CELL_SIZE * BOMBING_SIZE };
	// ���� ���� ������
	if (monster_head) {
		MONSTERNODE* q = monster_head;
		while (q->next != NULL) {
			RECT m_rt = GetStageRect(q->data.P.x, q->data.P.y);
			if (IntersectRect(&dest, &b_rt, &m_rt)) {
				if (q->data.name[0] == 'R' && !q->data.skill) {
					q->data.HP = MONSTER_HP;
					InsertMonster(&respawn_head, q->data);
				}
				else {
					// ��� ����Ʈ ��ü �߰�
					BOMB dead_data;
					InitBomb(&dead_data, q->data);
					InsertBomb(&dead_head, dead_data);
				}
				// ���� ����
				q = DelMonster(&monster_head, q->data.name);
				if (q->next == NULL) break;
			}
			q = q->next;
		}
		RECT m_rt = GetStageRect(q->data.P.x, q->data.P.y);
		if (IntersectRect(&dest, &b_rt, &m_rt)) {
			if (q->data.name[0] == 'R' && !q->data.skill) {
				q->data.HP = MONSTER_HP;
				InsertMonster(&respawn_head, q->data);
			}
			else {
				// ��� ����Ʈ ��ü �߰�
				BOMB dead_data;
				InitBomb(&dead_data, q->data);
				InsertBomb(&dead_head, dead_data);
			}
			// ���� ����
			q = DelMonster(&monster_head, q->data.name);
		}
	}

	RECT player_rt = GetStageRect(player.P.x, player.P.y);
	if (IntersectRect(&dest, &b_rt, &player_rt)) {
		player.life--;
		if (player.life == 0) {
			// ���� ����ó��
			PostQuitMessage(0);
		}
		while (1) {
			MONSTER m = {};
			player.P = { rand() % ((BOARD_SIZE - 2) * CELL_SIZE) + CELL_SIZE, rand() % ((BOARD_SIZE - 2) * CELL_SIZE + CELL_SIZE) };
			// ��ǥ�� ��ֹ��̸� �ٽ� ����
			if (!MonsterConflict(player.P, m)) {
				break;
			}
		}
	}
}

// ����
void InitBoss(HWND hWnd)
{
	bossframe_numW = 0;
	bossframe_numL = 2;

	// �Ѿ� �̵� Ÿ�̸�
	SetTimer(hWnd, BOSSGUN_MOVE, 1000, reinterpret_cast<TIMERPROC>(GunMove));

	boss.P.x = 240;  // ������ x ��ǥ
	boss.P.y = 390;  // ������ y ��ǥ
	boss.hp = 100;	// ���� ü��
}

void BossShoot()
{
	// �÷��̾�� ������ �Ÿ� ���
	int dx = player.P.x - boss.P.x;
	int dy = player.P.y - boss.P.y;


	for (int i = 0; i < bossgun_num; i++)
	{
		// �Ѿ��� �̵� ���� ���
		double angle = atan2(dy, dx);
		bossgun[i].B.x = cos(angle) * BOSSGUN_SPEED;
		bossgun[i].B.y = sin(angle) * BOSSGUN_SPEED;

		// �Ѿ� �ʱ� ��ġ ����
		bossgun[i].P.x = boss.P.x;
		bossgun[i].P.y = boss.P.y;


		while (bossgun[i].P.y < 520)
		{
			bossgun[i].P.x += (int)bossgun[i].B.x;
			bossgun[i].P.y += (int)bossgun[i].B.y;
		}
	}
}

void MakeBossBullet()
{
	// �Ѿ� �߰�
	bossgun_num++;

	// �����Ҵ�
	if (bossgun == NULL)
		bossgun = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bossgun_num);
	else
	{
		BOSSGUN* temp = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bossgun_num);
		memcpy(temp, bossgun, sizeof(BOSSGUN) * bossgun_num);
		free(bossgun);
		bossgun = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bossgun_num);
		memcpy(bossgun, temp, sizeof(BOSSGUN) * bossgun_num);
		free(temp);
	}

}

void MakeBigBossBullet()
{
	// �Ѿ� �߰�
	bigbossgun_num++;

	// �����Ҵ�
	if (bossgun == NULL)
		bossgun = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bigbossgun_num);
	else
	{
		BOSSGUN* temp = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bigbossgun_num);
		memcpy(temp, bossgun, sizeof(BOSSGUN) * bigbossgun_num);
		free(bossgun);
		bossgun = (BOSSGUN*)malloc(sizeof(BOSSGUN) * bigbossgun_num);
		memcpy(bossgun, temp, sizeof(BOSSGUN) * bigbossgun_num);
		free(temp);
	}
}

void CALLBACK BossFollowPlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{

	if (boss.P.x < player.P.x)
	{
		boss.P.x += 10;
		bossframe_numL = 0;
		bossframe_numW = (boss.frame.Rframe) % BOSS_SPRITE_WCNT + BOSS_SPRITE_WCNT;
		boss.frame.Rframe++;
	}
	else if (boss.P.x > player.P.x)
	{
		boss.P.x -= 10;
		bossframe_numL = 1;
		bossframe_numW = (boss.frame.Lframe) % BOSS_SPRITE_WCNT + BOSS_SPRITE_WCNT;
		boss.frame.Lframe++;
	}
	else {
		boss.frame.Rframe = boss.frame.Lframe = 0;
		bossframe_numW = 0;
		bossframe_numL = 2;
	}

	// ȭ�� ����
	InvalidateRect(hWnd, NULL, false);
}

void CALLBACK GunMove(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	// �Ѿ� ��ġ ����
	for (int i = 0; i < bossgun_num; i++)
	{
		bossgun[i].P.x += BOSSGUN_SPEED * bossgun[i].B.x;
		bossgun[i].P.y += BOSSGUN_SPEED * bossgun[i].B.y;
	}

	//Paint ȣ��
	InvalidateRect(hWnd, NULL, false);
}

// ���� �Ѿ� ���� �Լ�
void BossBulletSkill(PLAYER player, BOSSGUN* bossgun)
{
	for (int i = 0; i < bossgun_num; i++)
	{
		if (player.P.y == bossgun[i].B.y)
		{
			for (int j = 0; j < 4; i++)
			{
				bossgun[j].B.x = bossgun[i].B.x;
				bossgun[j].B.y = bossgun[i].B.y - 1;

				bossgun[j].B.x = bossgun[i].B.x;
				bossgun[j].B.y = bossgun[i].B.y + 1;

				bossgun[j].B.x = bossgun[i].B.x - 1;
				bossgun[j].B.y = bossgun[i].B.y;

				bossgun[j].B.x = bossgun[i].B.x + 1;
				bossgun[j].B.y = bossgun[i].B.y;
			}

			BossShoot();
		}
	}
}

void CALLBACK BossBigBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	MakeBigBossBullet();
	BossBulletSkill(player, bossgun);
}

// �Ѿ� �߻�
void CALLBACK Shooting(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	MakeBossBullet();
	BossShoot();
}

OBSTACLE* RandomLocateObstacle(OBSTACLE* obstacles)
{
	POINT rand_p = {};
	while (1) {
		MONSTER m = {};
		rand_p = { rand() % (BOARD_SIZE * CELL_SIZE), rand() % (5 * CELL_SIZE) };
		// ��ǥ�� ��ֹ��̸� �ٽ� ����
		if (!MonsterConflict(rand_p, m)) {
			break;
		}
	}
	
	// �迭 �ø���
	if ((obstacles)) {
		OBSTACLE* temp = (OBSTACLE*)malloc(sizeof(OBSTACLE) * obstacle_num);
		memcpy(temp, obstacles, sizeof(OBSTACLE) * obstacle_num);
		free(obstacles);
		obstacles = (OBSTACLE*)malloc(sizeof(OBSTACLE) * obstacle_num);
		memcpy(obstacles, temp, sizeof(OBSTACLE) * obstacle_num);
		free(temp);
	}
	else {
		obstacles = (OBSTACLE*)malloc(sizeof(OBSTACLE) * obstacle_num);
	}

	obstacles[obstacle_num] = {};
	obstacles[obstacle_num].kind = WOOD_ID;
	obstacles[obstacle_num++].p = rand_p;

	return obstacles;
}

void CALLBACK ObstacleInstallation(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	boss.skill = true;
	obstacles = RandomLocateObstacle(obstacles);
}
