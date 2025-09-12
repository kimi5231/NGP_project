#ifndef _HEADER1_H_
#define _HEADER1_H_
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fmod.hpp"
#include "fmod_errors.h"

#define START_MENU 10
#define IN_GAME 20
#define GAME_OVER 30

#define FONT_SIZE 50

#define CELL_SIZE 30
#define BOARD_SIZE 16

#define STAGE_COUNT 5
#define ANIMATION_SPEED 60

#define COWBOY_SPRITE_WCNT 8
#define COWBOY_SPRITE_LCNT 10

#define ANIMATION_SPEED 10
#define PLAYER_SPEED 5
#define BULLET_SPEED 10

// ����
#define MERCHANT_FRAME_WIDTH 120
#define MERCHANT_FRAME_HEIGHT 30
#define MERCHANT_SPRITE_CNT 4

#define ITEM_NUM 30

// ����
#define MONSTER_SPPED 3
#define MONSTER_SPRITE_WCNT 8
#define MONSTER_SPRITE_HCNT 4

#define NORMAL_MONSTER 0
#define RESPAWN_MONSTER 10
#define TANK_MONSTER 20
#define OBSTACLE_MONSTER 30
#define BOOMER_MONSTER 40

#define MONSTER_HP 5

// ����
#define BOSS_SPRITE_WCNT 8
#define BOSS_SPRITE_LCNT 4
#define BOSSGUN_SPEED 12

// Ÿ�̸� ID
#define BULLET_MOVE 1
#define ITEM_DURATION 2
#define EFFECT_LOAD 3
#define STAGE_ANIMATION 4
#define MONSTER_PRODUSE 5
#define MONSTER_MOVE 6
#define MONSTER_SKILL 7
#define MERCHANT_WALKING 8
#define STAGE_START 9

#define BOSSGUN_MOVE 10
#define BOSS_MOVE 11
#define BOSS_SHOOT 12
#define BOSS_BIGBULLET 13
#define BOSS_OBSTACLE 14

#define STAGE_TIME 10

// ��ź
#define BOMB_TIME 100
#define BOMB_SPRITE_CNT 6

#define BOMBING_SPRITE_CNT 7
#define BOMBING_SIZE 3

// ��� ����Ʈ
#define DEAD_SPRITE_CNT 18

// ��ֹ�
#define WALL_OBSTACLE_CNT 48
#define OBSTACLE_SPEED 5
#define OBSTACLE_SPRITE_WCNT 3
#define OBSTACLE_SPRITE_HCNT 4

#define WALL_ID 10
#define WOOD_ID 20
#define MOVE_ID 30

typedef struct ShopItem {
	TCHAR name[50];
	int price;
} SHOPITEM;

typedef struct ITEM
{
	POINT P;
	int num;
	bool show;
	bool effect;
	int use_cnt;
	int effect_cnt;
}ITEM;

typedef struct PLAYER
{
	POINT P;
	int ATP;
	int speed;
	int life;
	int coin;
	bool item;
	bool bullet;
	int item_idx;
	SHOPITEM shopt_item;
	ITEM effect_item;
}PLAYER;

typedef struct Frame
{
	int Lframe, Rframe, Uframe, Dframe;
}FRAME;


typedef struct Merchant
{
	POINT p;
	FRAME frame;
	int current_frame;
}MERCHANT;

typedef struct BULLET
{
	POINT P;
	POINT direct;
}BULLET;

typedef struct itemnum
{
	int num;
	bool is_end;
}ITEMNUM;

// ���� ����ü
typedef struct Monster
{
	TCHAR name[20];
	POINT P;
	POINT direct;
	FRAME frame;
	int current_frame;
	int HP;
	bool skill;
	POINT target;
	int time_cnt;
}MONSTER;

typedef struct Monster_Node
{
	MONSTER data;
	struct Monster_Node* next;
}MONSTERNODE;

// ��ź ����ü
typedef struct Bomb
{
	POINT P;
	int cnt;
	int current_frame;
}BOMB;

typedef struct BombNode
{
	BOMB data;
	struct BombNode* next;
}BOMBNODE;

// ��ֹ� ����ü
typedef struct Obstacle
{
	POINT p;
	POINT direct;
	POINT start_p;
	int kind;
	int speed;
	int current_frame;
}OBSTACLE;

// ���� ����ü
typedef struct BOSS
{
	POINT P;
	Frame frame;
	int hp;
	bool skill;
}BOSS;

// ���� �Ѿ� ����ü
typedef struct BOSSGUN
{
	POINT P;
	POINT B;
	POINT G;
}BOSSGUN;

// window ���ν���
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// ���� �Լ�
void SoundSetting(void);

// �޴� �Լ�
void StartMenu(HDC hDC, RECT rt, RECT menu_start_rt, RECT menu_exit_rt);
bool Menu_Is_Click(POINT m, RECT menu_start_rt, RECT menu_exit_rt);

// �� ���� rect ��� �Լ�
RECT GetStageRect(int x, int y);

void TransitionToNextStage(HWND);
void CALLBACK AnimateStageTransition(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

// ���� UI ���� �Լ�
void GameUI(PLAYER player, HDC hdc);
// ���� ������ UI �Լ�
void GameUIItem(ITEM, HDC);
// �������� ���� �� ���� ����
void EndStage();
// ���� �ʱ�ȭ �Լ�
void InitMerchant();
// ���� �׸��� �Լ�
void DrawMerchant(HDC memDC);

// �������� �����ϴ� �Լ�
void BuyItem(PLAYER* player, TCHAR* itemname, int itemprice);
void GetShopItem(int Px, int Py, SHOPITEM shopitem);
// ���� UI ���� �Լ�
void ShopUI(HDC memdc);
// ���� Ÿ�̸�
void CALLBACK MerchantWalking(HWND, UINT, UINT_PTR, DWORD);
void ShopItem(HDC memDC, HDC memDCImage);

// ������ ���� Ÿ�̸� ���ν���
void CALLBACK CountItemTime(HWND, UINT, UINT, DWORD);
// ������ ��Ʈ�� �ε� �Լ�
void MakeItem(HWND, POINT);
// ������ ��Ʈ�� ��� �Լ�
void ShowItem(ITEM item, HDC memDC, HDC memDCImage);
// ������ ȹ�� �Լ�
ITEM GetItem(int Px, int Py, ITEM item, int idx);
// ������ ��� �Լ�
void UseItem(HWND);

//����Ʈ ��Ʈ�� �ε� �Լ�
void CALLBACK EffectLoad(HWND, UINT, UINT, DWORD);
// ����Ʈ ��Ʈ�� ��� �Լ�
void ShowEffect(ITEM item, HDC memDC, HDC memDCImage);
// ����Ʈ ��Ʈ�ʸ� �ε� �Լ�
void EffectBitmapLoad();

// ��Ʈ�� �ε� �Լ�
void LoadAllBitmap();
// ��� ��Ʈ�� ���� �Լ�
void UnLoadAllBitmap();
// ��ֹ� �� ĭ ��� �Լ�
void DrawOneObstacle(HBITMAP, HBITMAP, OBSTACLE, HDC, RECT);
// ��ֹ� ���� ��� �Լ�
void DrawOneObstacleMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt);
// ��Ȱ ���� ��� �Լ�
void DrawRespawnMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt);
// ��� ��Ʈ��, �ִϸ��̼� ��� �Լ�
void DrawBitmap(HDC, RECT);

// �÷��̾� �ʱ�ȭ �Լ�
void InitPlayer(HWND);
// �÷��̾� ������Ʈ ��� �Լ�
bool PlayerBlockObject();

// �Ѿ� �̵� Ÿ�̸� �Լ�
void CALLBACK BulletMove(HWND, UINT, UINT, DWORD);
// �Ѿ� ���� �Լ�
void MakeBullet(void);
// �Ѿ� ���� ���� �Լ�
void SetDirect(int);
// �Ѿ� ���� �Լ�
void DelBullet(void);
// �Ѿ�, ��ֹ� �浹üũ �Լ�
bool BlockBullet(int);
// ���� �Ѿ� �Լ�
void ShotgunBullet(int, int, int);
// �������� �Ѿ� �Լ�
void WaterWheelBullet(void);

// ���� ������ �ʱ�ȭ �Լ�
void initMonster(MONSTER* data, PLAYER player);
// ���� tail ��� ���� �Լ�
MONSTERNODE* Gettail(MONSTERNODE* head);
// ���� ��� ���� �Լ�
void InsertMonster(MONSTERNODE** head, MONSTER data);
// ���� ID Ž�� �Լ�
MONSTERNODE* SearchNode(MONSTERNODE* head, MONSTERNODE* node, TCHAR* name);
// ���� ��� ���� �Լ�
MONSTERNODE* DelMonster(MONSTERNODE** head, TCHAR* name);

// ��ź ������ �ʱ�ȭ �Լ�
void InitBomb(BOMB* bombdata, MONSTER monster);
// ��ź tail ��� ���� �Լ�
BOMBNODE* Bomb_Gettail(BOMBNODE* head);
// ��ź ��� ���� �Լ�
void InsertBomb(BOMBNODE** head, BOMB data);
// ��ź ID Ž�� �Լ�
BOMBNODE* Bomb_SearchNode(BOMBNODE* head, BOMBNODE* node, int frame);
// ��ź ��� ���� �Լ�
BOMBNODE* DelBomb(BOMBNODE** head, int frame);

// ��ź �̵� �Լ�
void KickBomb();
// ���� �̵� �Լ�
void OneMonsterMove(MONSTERNODE** node, POINT target);
// ���� ��Ʈ�� ��� �Լ�
void DrawOneMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC hDC, RECT rt);
// ���� �̵� ���ν���
void CALLBACK MonsterMove(HWND, UINT, UINT, DWORD);
// ���� �浹 �Լ�
bool MonsterConflict(POINT p, MONSTER monster);
// ���� ��ų Ÿ�� �Լ�
void MonsterSkillCnt(MONSTER* data);

// ��ź ��� �Լ�
void DrawOneBombAndDead(BOMB bomb, HBITMAP hbomb, HBITMAP hbomb_mask, HDC memDC, RECT rt, int sprite_cnt);
// ��ֹ� �ʱ�ȭ
OBSTACLE* InitObstalce(OBSTACLE* obstacles);
// ���� �浹 üũ �Լ�
void BombingDamage(BOMB bombing);

// ���� �ʱ�ȭ �Լ�
void InitBoss(HWND);

// �� ��� ���� �Լ�
void BossShoot();
// ���� ��ų �׸���
void DrawBossSkill(HDC memDC);
// �Ѿ� ���� �Լ�
void MakeBossBullet();
// ���� �Ѿ� ���� �Լ�
void MakeBigBossBullet();
// ������ �÷��̾� �¿� ������ ���� �̵��ϴ� �Լ�
void CALLBACK BossFollowPlayer(HWND, UINT, UINT, DWORD);
// ���� �Ѿ� �̵� �Լ�
void CALLBACK GunMove(HWND, UINT, UINT, DWORD);
// ���� �Ѿ� �߻� �Լ�
void CALLBACK Shooting(HWND, UINT, UINT, DWORD);
// ���� ��ֹ� ���� ��ġ ���� �Լ�
OBSTACLE* RandomLocateObstacle(OBSTACLE*);
// ���� ��ֹ� ��ġ �Լ�
void CALLBACK ObstacleInstallation(HWND, UINT, UINT, DWORD);
// ���� �Ѿ� ���� �Լ�
void BossBulletSkill(PLAYER, BOSSGUN*);
// ���� ���� �Ѿ� �߻� �Լ�
void CALLBACK BossBigBullet(HWND, UINT, UINT, DWORD);

#endif