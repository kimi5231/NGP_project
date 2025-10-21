#ifndef _HEADER1_H_
#define _HEADER1_H_
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include "fmod.hpp"
//#include "fmod_errors.h"

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

// 상인
#define MERCHANT_FRAME_WIDTH 120
#define MERCHANT_FRAME_HEIGHT 30
#define MERCHANT_SPRITE_CNT 4

#define ITEM_NUM 30

// 몬스터
#define MONSTER_SPPED 3
#define MONSTER_SPRITE_WCNT 8
#define MONSTER_SPRITE_HCNT 4

#define NORMAL_MONSTER 0
#define RESPAWN_MONSTER 10
#define TANK_MONSTER 20
#define OBSTACLE_MONSTER 30
#define BOOMER_MONSTER 40

#define MONSTER_HP 5

// 보스
#define BOSS_SPRITE_WCNT 8
#define BOSS_SPRITE_LCNT 4
#define BOSSGUN_SPEED 12

// 타이머 ID
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

// 폭탄
#define BOMB_TIME 100
#define BOMB_SPRITE_CNT 6

#define BOMBING_SPRITE_CNT 7
#define BOMBING_SIZE 3

// 사망 이펙트
#define DEAD_SPRITE_CNT 18

// 장애물
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

// 몬스터 구조체
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

// 폭탄 구조체
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

// 장애물 구조체
typedef struct Obstacle
{
	POINT p;
	POINT direct;
	POINT start_p;
	int kind;
	int speed;
	int current_frame;
}OBSTACLE;

// 보스 구조체
typedef struct BOSS
{
	POINT P;
	Frame frame;
	int hp;
	bool skill;
}BOSS;

// 보스 총알 구조체
typedef struct BOSSGUN
{
	POINT P;
	POINT B;
	POINT G;
}BOSSGUN;

// window 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 사운드 함수
void SoundSetting(void);

// 메뉴 함수
void StartMenu(HDC hDC, RECT rt, RECT menu_start_rt, RECT menu_exit_rt);
bool Menu_Is_Click(POINT m, RECT menu_start_rt, RECT menu_exit_rt);

// 맵 기준 rect 얻는 함수
RECT GetStageRect(int x, int y);

void TransitionToNextStage(HWND);
void CALLBACK AnimateStageTransition(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

// 게임 UI 갱신 함수
void GameUI(PLAYER player, HDC hdc);
// 게임 아이템 UI 함수
void GameUIItem(ITEM, HDC);
// 스테이지 종료 시 상점 오픈
void EndStage();
// 상인 초기화 함수
void InitMerchant();
// 상인 그리기 함수
void DrawMerchant(HDC memDC);

// 아이템을 구매하는 함수
void BuyItem(PLAYER* player, TCHAR* itemname, int itemprice);
void GetShopItem(int Px, int Py, SHOPITEM shopitem);
// 상점 UI 갱신 함수
void ShopUI(HDC memdc);
// 상인 타이머
void CALLBACK MerchantWalking(HWND, UINT, UINT_PTR, DWORD);
void ShopItem(HDC memDC, HDC memDCImage);

// 아이템 지속 타이머 프로시저
void CALLBACK CountItemTime(HWND, UINT, UINT, DWORD);
// 아이템 비트맵 로드 함수
void MakeItem(HWND, POINT);
// 아이템 비트맵 출력 함수
void ShowItem(ITEM item, HDC memDC, HDC memDCImage);
// 아이템 획득 함수
ITEM GetItem(int Px, int Py, ITEM item, int idx);
// 아이템 사용 함수
void UseItem(HWND);

//이펙트 비트맵 로드 함수
void CALLBACK EffectLoad(HWND, UINT, UINT, DWORD);
// 이펙트 비트맵 출력 함수
void ShowEffect(ITEM item, HDC memDC, HDC memDCImage);
// 이펙트 비트맵만 로드 함수
void EffectBitmapLoad();

// 비트맵 로드 함수
void LoadAllBitmap();
// 배경 비트맵 삭제 함수
void UnLoadAllBitmap();
// 장애물 한 칸 출력 함수
void DrawOneObstacle(HBITMAP, HBITMAP, OBSTACLE, HDC, RECT);
// 장애물 몬스터 출력 함수
void DrawOneObstacleMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt);
// 부활 몬스터 출력 함수
void DrawRespawnMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt);
// 모든 비트맵, 애니메이션 출력 함수
void DrawBitmap(HDC, RECT);

// 플레이어 초기화 함수
void InitPlayer(HWND);
// 플레이어 오브젝트 블락 함수
bool PlayerBlockObject();

// 총알 이동 타이머 함수
void CALLBACK BulletMove(HWND, UINT, UINT, DWORD);
// 총알 생성 함수
void MakeBullet(void);
// 총알 방향 지정 함수
void SetDirect(int);
// 총알 삭제 함수
void DelBullet(void);
// 총알, 장애물 충돌체크 함수
bool BlockBullet(int);
// 샷건 총알 함수
void ShotgunBullet(int, int, int);
// 물레바퀴 총알 함수
void WaterWheelBullet(void);

// 몬스터 데이터 초기화 함수
void initMonster(MONSTER* data, PLAYER player);
// 몬스터 tail 노드 리턴 함수
MONSTERNODE* Gettail(MONSTERNODE* head);
// 몬스터 노드 삽입 함수
void InsertMonster(MONSTERNODE** head, MONSTER data);
// 몬스터 ID 탐색 함수
MONSTERNODE* SearchNode(MONSTERNODE* head, MONSTERNODE* node, TCHAR* name);
// 몬스터 노드 삭제 함수
MONSTERNODE* DelMonster(MONSTERNODE** head, TCHAR* name);

// 폭탄 데이터 초기화 함수
void InitBomb(BOMB* bombdata, MONSTER monster);
// 폭탄 tail 노드 리턴 함수
BOMBNODE* Bomb_Gettail(BOMBNODE* head);
// 폭탄 노드 삽입 함수
void InsertBomb(BOMBNODE** head, BOMB data);
// 폭탄 ID 탐색 함수
BOMBNODE* Bomb_SearchNode(BOMBNODE* head, BOMBNODE* node, int frame);
// 폭탄 노드 삭제 함수
BOMBNODE* DelBomb(BOMBNODE** head, int frame);

// 폭탄 이동 함수
void KickBomb();
// 몬스터 이동 함수
void OneMonsterMove(MONSTERNODE** node, POINT target);
// 몬스터 비트맵 출력 함수
void DrawOneMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC hDC, RECT rt);
// 몬스터 이동 프로시저
void CALLBACK MonsterMove(HWND, UINT, UINT, DWORD);
// 몬스터 충돌 함수
bool MonsterConflict(POINT p, MONSTER monster);
// 몬스터 스킬 타임 함수
void MonsterSkillCnt(MONSTER* data);

// 폭탄 출력 함수
void DrawOneBombAndDead(BOMB bomb, HBITMAP hbomb, HBITMAP hbomb_mask, HDC memDC, RECT rt, int sprite_cnt);
// 장애물 초기화
OBSTACLE* InitObstalce(OBSTACLE* obstacles);
// 폭발 충돌 체크 함수
void BombingDamage(BOMB bombing);

// 보스 초기화 함수
void InitBoss(HWND);

// 총 쏘는 방향 함수
void BossShoot();
// 보스 스킬 그리기
void DrawBossSkill(HDC memDC);
// 총알 생성 함수
void MakeBossBullet();
// 분할 총알 생성 함수
void MakeBigBossBullet();
// 보스가 플레이어 좌우 움직임 따라 이동하는 함수
void CALLBACK BossFollowPlayer(HWND, UINT, UINT, DWORD);
// 보스 총알 이동 함수
void CALLBACK GunMove(HWND, UINT, UINT, DWORD);
// 보스 총알 발사 함수
void CALLBACK Shooting(HWND, UINT, UINT, DWORD);
// 보스 장애물 랜덤 위치 설정 함수
OBSTACLE* RandomLocateObstacle(OBSTACLE*);
// 보스 장애물 설치 함수
void CALLBACK ObstacleInstallation(HWND, UINT, UINT, DWORD);
// 보스 총알 분해 함수
void BossBulletSkill(PLAYER, BOSSGUN*);
// 보스 분해 총알 발사 함수
void CALLBACK BossBigBullet(HWND, UINT, UINT, DWORD);

#endif