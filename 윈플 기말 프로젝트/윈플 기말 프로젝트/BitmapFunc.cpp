#include "Header.h"

extern HBITMAP wall_wood_block;
extern HBITMAP wood_block;
extern HBITMAP wood_block_mask;
extern HBITMAP hStageBitmap[5];
extern HBITMAP hCellBitmap;
extern HBITMAP hObstacleBitmap;
extern MERCHANT merchant;
extern bool isShopOpen;

extern HBITMAP cowboy, cowboy_mask;
extern HBITMAP item_image, item_mask;
extern HBITMAP move_obstacle;

extern HBITMAP coffee_effect;
extern HBITMAP bullet_effect, bullet_effect_mask;
extern HBITMAP shotgun_effect, shotgun_effect_mask;
extern HBITMAP waterwheel_effect, waterwheel_effect_mask;
extern HBITMAP hourglass_effect;
extern HBITMAP lightning_effect, lightning_effect_mask, lightning_effect_background;

extern HBITMAP normal_monster;
extern HBITMAP tank_monster;
extern HBITMAP respawn_monster;
extern HBITMAP obstacle_monster;
extern HBITMAP boomer_monster;

extern HBITMAP normal_monster_mask;
extern HBITMAP tank_monster_mask;
extern HBITMAP respawn_monster_mask;
extern HBITMAP obstacle_monster_mask;
extern HBITMAP boomer_monster_mask;
extern OBSTACLE* obstacles;
extern HBITMAP move_obstacle_mask;

extern HBITMAP bomb;
extern HBITMAP bomb_mask;
extern HBITMAP bombing;
extern HBITMAP bombing_mask;

extern HBITMAP dead_effect;
extern HBITMAP dead_effect_mask;

extern PLAYER player;
extern BULLET* bullet;
extern ITEM item[ITEM_NUM];
extern ITEMNUM item_num;

extern MONSTERNODE* monster_head;
extern MONSTERNODE* respawn_head;
extern BOMBNODE* bomb_head;
extern BOMBNODE* bombing_head;
extern BOMBNODE* dead_head;
extern OBSTACLE* obstacles;
extern int obstacle_num;

extern int currentStage;
extern int stageIndex;
extern int animationOffset;
extern int play_time;

extern int frame_numW, frame_numL;

extern int bullet_num;
extern int bullet_speed;
extern bool isEntering;

//extern bool isBossStage;

extern HBITMAP boss_sprite, boss_mask;
extern BOSS boss;
extern BOSSGUN* bossgun;
extern int bossgun_num;
extern int bigbossgun_num;
extern int bossframe_numW, bossframe_numL;
extern int StartX, StartY, EndX, EndY;

extern HINSTANCE g_hInst;

void LoadAllBitmap()
{
    // ��� ��Ʈ�� �ε�
    hCellBitmap = (HBITMAP)LoadImage(g_hInst, L"sand_background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    // ��ֹ� ��Ʈ�� �ε�
    wall_wood_block = (HBITMAP)LoadImage(g_hInst, TEXT("wall_wood_block.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    wood_block = (HBITMAP)LoadImage(g_hInst, TEXT("wood_block.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    wood_block_mask = (HBITMAP)LoadImage(g_hInst, TEXT("wood_block_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    // �÷��̾� ��Ʈ�� �ε�
    cowboy = (HBITMAP)LoadImage(g_hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    cowboy_mask = (HBITMAP)LoadImage(g_hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
    
    // ���� ��Ʈ�� �ε�
    normal_monster = (HBITMAP)LoadImage(g_hInst, TEXT("�Ϲݸ���.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    tank_monster = (HBITMAP)LoadImage(g_hInst, TEXT("tank.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    respawn_monster = (HBITMAP)LoadImage(g_hInst, TEXT("respawn_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    obstacle_monster = (HBITMAP)LoadImage(g_hInst, TEXT("obstacle_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    boomer_monster = (HBITMAP)LoadImage(g_hInst, TEXT("boomer.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    normal_monster_mask = (HBITMAP)LoadImage(g_hInst, TEXT("�Ϲݸ���_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    tank_monster_mask = (HBITMAP)LoadImage(g_hInst, TEXT("tank_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    respawn_monster_mask = (HBITMAP)LoadImage(g_hInst, TEXT("respawn_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    obstacle_monster_mask = (HBITMAP)LoadImage(g_hInst, TEXT("obstacle_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    boomer_monster_mask = (HBITMAP)LoadImage(g_hInst, TEXT("boomer_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    bomb = (HBITMAP)LoadImage(g_hInst, TEXT("bomb.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    bomb_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bomb_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    bombing = (HBITMAP)LoadImage(g_hInst, TEXT("bomb_effect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    bombing_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bomb_effect_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    dead_effect = (HBITMAP)LoadImage(g_hInst, TEXT("dead_effect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    dead_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("dead_effect_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    move_obstacle = (HBITMAP)LoadImage(g_hInst, TEXT("baby_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    move_obstacle_mask = (HBITMAP)LoadImage(g_hInst, TEXT("baby_move_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    // ���� ��Ʈ�� �ε�
    boss_sprite = (HBITMAP)LoadImage(g_hInst, TEXT("boss.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    boss_mask = (HBITMAP)LoadImage(g_hInst, TEXT("boss_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void StartMenu(HDC hDC, RECT rt, RECT menu_start_rt, RECT menu_exit_rt)
{
    HDC memDC, memDCImage;
    HBITMAP oldbit[2], hbit, hbackground;
    BITMAP back_bmp;
    TCHAR text[30] = L"START";
    HFONT hfont, oldfont;
    RECT shadow_rt;

    memDC = CreateCompatibleDC(hDC);
    memDCImage = CreateCompatibleDC(memDC);

    hbit = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
    oldbit[0] = (HBITMAP)SelectObject(memDC, hbit);

    // ��� �ε�
    hbackground = (HBITMAP)LoadImage(g_hInst, TEXT("���ι��_��Ӱ�.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    // �̹��� ũ��
    GetObject(hbackground, sizeof(BITMAP), &back_bmp);
    oldbit[1] = (HBITMAP)SelectObject(memDCImage, hbackground);

    // memDC�� �̹��� ���
    StretchBlt(memDC, rt.left, rt.top, rt.right, rt.bottom, memDCImage, 0, 0, back_bmp.bmWidth, back_bmp.bmHeight, SRCCOPY);

    // �޴� ����
    hfont = (HFONT)CreateFont(FONT_SIZE, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"�ü�");
    oldfont = (HFONT)SelectObject(memDC, hfont);
    SetBkMode(memDC, TRANSPARENT);

    // start font ���
    shadow_rt = { menu_start_rt.left + 5,menu_start_rt.top + 5, menu_start_rt.right + 5, menu_start_rt.bottom + 5 };
    SetTextColor(memDC, RGB(192, 192, 192));
    DrawText(memDC, text, lstrlen(text), &shadow_rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    SetTextColor(memDC, RGB(0, 0, 0));
    DrawText(memDC, text, lstrlen(text), &menu_start_rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


    // exit font ���
    _tcscpy_s(text, L"EXIT");
    shadow_rt = { menu_exit_rt.left + 5,menu_exit_rt.top + 5, menu_exit_rt.right + 5, menu_exit_rt.bottom + 5 };
    SetTextColor(memDC, RGB(192, 192, 192));
    DrawText(memDC, text, lstrlen(text), &shadow_rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    SetTextColor(memDC, RGB(0, 0, 0));
    DrawText(memDC, text, lstrlen(text), &menu_exit_rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    // hDC ���(���� ���)
    BitBlt(hDC, rt.left, rt.top, rt.right, rt.bottom, memDC, 0, 0, SRCCOPY);

    SelectObject(memDCImage, oldfont);
    DeleteObject(hfont);
    SelectObject(memDC, oldbit[0]);
    SelectObject(memDCImage, oldbit[1]);
    DeleteObject(hbit);
    DeleteObject(hbackground);
    DeleteDC(memDC);
    DeleteDC(memDCImage);
}

bool Menu_Is_Click(POINT m, RECT menu_start_rt, RECT menu_exit_rt)
{
    if (PtInRect(&menu_start_rt, m)) {
        return true;
    }
    else if (PtInRect(&menu_exit_rt, m)) {
        PostQuitMessage(0);
        return false;
    }

    return false;
}

void UnLoadAllBitmap()
{
    DeleteObject(hCellBitmap);

    DeleteObject(wall_wood_block);
    DeleteObject(wood_block);
    DeleteObject(wood_block_mask);

    DeleteObject(cowboy);
    DeleteObject(cowboy_mask);

    DeleteObject(normal_monster);
    DeleteObject(tank_monster);
    DeleteObject(respawn_monster);
    DeleteObject(obstacle_monster);
    DeleteObject(boomer_monster);

    DeleteObject(normal_monster_mask);
    DeleteObject(tank_monster_mask);
    DeleteObject(respawn_monster_mask);
    DeleteObject(obstacle_monster_mask);
    DeleteObject(boomer_monster_mask);

    DeleteObject(bomb);
    DeleteObject(bomb_mask);
    DeleteObject(bombing);
    DeleteObject(bombing_mask);
    DeleteObject(dead_effect);
    DeleteObject(dead_effect_mask);
    DeleteObject(move_obstacle);
    DeleteObject(move_obstacle_mask);

    DeleteObject(boss_sprite);
    DeleteObject(boss_mask);
}

void DrawMerchant(HDC memDC)
{
    HDC memdcImage = CreateCompatibleDC(memDC);

    int spriteX = (merchant.current_frame % (MERCHANT_SPRITE_CNT / 2)) * CELL_SIZE;
    int spriteY = 0;

    // ���� ��Ʈ�� �ε�
    HBITMAP merchantBitmap = (HBITMAP)LoadImage(g_hInst, TEXT("merchant_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    HBITMAP merchantmask = (HBITMAP)LoadImage(g_hInst, TEXT("merchant_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    spriteX = (merchant.current_frame % MERCHANT_SPRITE_CNT) * CELL_SIZE;
    SelectObject(memdcImage, merchantmask);
    BitBlt(memDC, merchant.p.x, merchant.p.y, CELL_SIZE, CELL_SIZE, memdcImage, spriteX, spriteY, SRCAND);
    SelectObject(memdcImage, merchantBitmap);
    BitBlt(memDC, merchant.p.x, merchant.p.y, CELL_SIZE, CELL_SIZE, memdcImage, spriteX, spriteY, SRCPAINT);

    DeleteObject(merchantBitmap);
    DeleteObject(merchantmask);
    DeleteDC(memdcImage);
}


void DrawBoss(BOSS boss, HDC memDC)
{
    int spriteX, spriteY;
    HDC memdcImage = CreateCompatibleDC(memDC);

    // sprite sheet ��ġ
    spriteX = (bossframe_numW % BOSS_SPRITE_WCNT) * CELL_SIZE;
    spriteY = bossframe_numL * CELL_SIZE;

    // ���� ����ũ
    SelectObject(memdcImage, boss_mask);
    BitBlt(memDC, boss.P.x, boss.P.y, CELL_SIZE, CELL_SIZE, memdcImage, spriteX, spriteY, SRCAND);
    // ����
    SelectObject(memdcImage, boss_sprite);
    BitBlt(memDC, boss.P.x, boss.P.y, CELL_SIZE, CELL_SIZE, memdcImage, spriteX, spriteY, SRCPAINT);

    // ���� �Ѿ� �� ��� �׸���
    HBRUSH hBrush = CreateSolidBrush(RGB(120, 120, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
    for (int i = 0; i < bossgun_num; i++)
        Ellipse(memDC, bossgun[i].P.x, bossgun[i].P.y, bossgun[i].P.x + 10, bossgun[i].P.y + 10);

    SelectObject(memDC, oldBrush);
    DeleteObject(hBrush);

    DeleteDC(memdcImage);
}

void DrawBossSkill(HDC memDC)
{
    HDC memdcImage = CreateCompatibleDC(memDC);

    HBRUSH hBrush = CreateSolidBrush(RGB(120, 120, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
    for (int i = 0; i < bigbossgun_num; i++)
        Ellipse(memDC, bossgun[i].G.x, bossgun[i].G.y, bossgun[i].G.x + 15, bossgun[i].G.y + 15);

    SelectObject(memDC, oldBrush);
    DeleteObject(hBrush);

    DeleteDC(memdcImage);
}

void ShopItem(HDC memDC, HDC memDCImage)
{
    if (isShopOpen)
    {
        HBITMAP shoe, shoe_mask;
        HBITMAP gun, gun_mask;
        HBITMAP slug, slug_mask;

        // ���� ������ �ε�
        shoe = (HBITMAP)LoadImage(g_hInst, TEXT("�Ź�.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        shoe_mask = (HBITMAP)LoadImage(g_hInst, TEXT("�Ź� ����ũ.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        gun = (HBITMAP)LoadImage(g_hInst, TEXT("��.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        gun_mask = (HBITMAP)LoadImage(g_hInst, TEXT("�� ����ũ.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        slug = (HBITMAP)LoadImage(g_hInst, TEXT("�Ѿ�.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        slug_mask = (HBITMAP)LoadImage(g_hInst, TEXT("�Ѿ� ����ũ.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        // �Ź�
        SelectObject(memDCImage, shoe_mask);
        BitBlt(memDC, 130, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);
        SelectObject(memDCImage, shoe);
        BitBlt(memDC, 130, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);

        // �Ѿ�
        SelectObject(memDCImage, slug_mask);
        BitBlt(memDC, 220, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);
        SelectObject(memDCImage, slug);
        BitBlt(memDC, 220, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);

        // ��
        SelectObject(memDCImage, gun_mask);
        BitBlt(memDC, 310, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);
        SelectObject(memDCImage, gun);
        BitBlt(memDC, 310, 160, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);

        DeleteObject(shoe);
        DeleteObject(shoe_mask);
        DeleteObject(gun);
        DeleteObject(gun_mask);
        DeleteObject(slug);
        DeleteObject(slug_mask);
    }

}

void DrawOneObstacle(HBITMAP hobstacle, HBITMAP hobstacle_mask, OBSTACLE obstacle, HDC memDC, RECT rt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int spriteX, spriteY;
    memDCImage = CreateCompatibleDC(memDC);
    // sprite sheet ��ġ
    spriteX = (obstacle.current_frame % OBSTACLE_SPRITE_WCNT) * CELL_SIZE;
    spriteY = obstacle.current_frame / OBSTACLE_SPRITE_WCNT * CELL_SIZE;

    // ��ֹ�
    oldbit = (HBITMAP)SelectObject(memDCImage, hobstacle_mask);
    BitBlt(memDC, obstacle.p.x, obstacle.p.y, CELL_SIZE,  CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);

    SelectObject(memDCImage, hobstacle);
    BitBlt(memDC, obstacle.p.x, obstacle.p.y , CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void DrawOneMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int spriteX, spriteY;

    memDCImage = CreateCompatibleDC(memDC);

    // sprite sheet ��ġ
    spriteX = (monster.current_frame % MONSTER_SPRITE_WCNT) * CELL_SIZE;
    spriteY = monster.current_frame / MONSTER_SPRITE_WCNT * CELL_SIZE;

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hmonster_mask);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);

    SelectObject(memDCImage, hmonster);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void DrawOneBombAndDead(BOMB bomb, HBITMAP hbomb, HBITMAP hbomb_mask, HDC memDC, RECT rt, int sprite_cnt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int sprite;

    memDCImage = CreateCompatibleDC(memDC);

    // sprite sheet ��ġ
    sprite = (bomb.current_frame % sprite_cnt) * CELL_SIZE;

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hbomb_mask);
    BitBlt(memDC, bomb.P.x, bomb.P.y, CELL_SIZE, CELL_SIZE, memDCImage, sprite, 0, SRCAND);

    SelectObject(memDCImage, hbomb);
    BitBlt(memDC, bomb.P.x, bomb.P.y, CELL_SIZE, CELL_SIZE, memDCImage, sprite, 0, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void DrawOneBombing(BOMB bomb, HBITMAP hbombing, HBITMAP hbombing_mask, HDC memDC, RECT rt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int sprite;

    memDCImage = CreateCompatibleDC(memDC);

    // sprite sheet ��ġ
    sprite = (bomb.current_frame % BOMBING_SPRITE_CNT) * (CELL_SIZE);

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hbombing_mask);
    BitBlt(memDC, bomb.P.x, bomb.P.y, CELL_SIZE * BOMBING_SIZE, CELL_SIZE * BOMBING_SIZE, memDCImage, sprite, 0, SRCAND);

    SelectObject(memDCImage, hbombing);
    BitBlt(memDC, bomb.P.x, bomb.P.y, CELL_SIZE * BOMBING_SIZE, CELL_SIZE * BOMBING_SIZE, memDCImage, sprite, 0, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void DrawOneObstacleMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int spriteX, spriteY;

    memDCImage = CreateCompatibleDC(memDC);

    // sprite sheet ��ġ
    spriteX = monster.current_frame / MONSTER_SPRITE_WCNT * CELL_SIZE;
    spriteY = 4 * CELL_SIZE;

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hmonster_mask);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);

    SelectObject(memDCImage, hmonster);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void DrawRespawnMonster(MONSTER monster, HBITMAP hmonster, HBITMAP hmonster_mask, HDC memDC, RECT rt)
{
    HDC memDCImage;
    HBITMAP oldbit;
    int spriteX, spriteY;

    memDCImage = CreateCompatibleDC(memDC);

    // ����
    // sprite sheet ��ġ
    spriteX = monster.current_frame / MONSTER_SPRITE_WCNT * CELL_SIZE;
    spriteY = 4 * CELL_SIZE;

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hmonster_mask);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);

    SelectObject(memDCImage, hmonster);
    BitBlt(memDC, monster.P.x, monster.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    // �Ӹ�
    // sprite sheet ��ġ
    if (monster.direct.x < 0) {
        if (monster.direct.y < 0) {
            spriteX = 3 * CELL_SIZE;
        }
        else if(monster.direct.y > 0){
            spriteX = 1 * CELL_SIZE;
        }
        else {
            spriteX = 2 * CELL_SIZE;
        }
    }
    else if(monster.direct.x > 0){
        if (monster.direct.y < 0) {
            spriteX = 5 * CELL_SIZE;
        }
        else if (monster.direct.y > 0) {
            spriteX = 7 * CELL_SIZE;
        }
        else {
            spriteX = 6 * CELL_SIZE;
        }
    }
    else {
        if (monster.direct.y < 0) {
            spriteX = 4 * CELL_SIZE;
        }
        else if (monster.direct.y > 0) {
            spriteX = 0 * CELL_SIZE;
        }
        else {
            spriteX = 4 * CELL_SIZE;
        }
    }
    spriteY = 5 * CELL_SIZE;

    POINT d = {};
    d.x = (monster.direct.x > 0) ? 1 : -1;
    d.y = (monster.direct.y > 0) ? 1 : -1;

    // ����ũ
    oldbit = (HBITMAP)SelectObject(memDCImage, hmonster_mask);
    BitBlt(memDC, monster.P.x + (d.x * CELL_SIZE / 2), monster.P.y + (d.y * CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);

    SelectObject(memDCImage, hmonster);
    BitBlt(memDC, monster.P.x + (d.x * CELL_SIZE / 2), monster.P.y + (d.y * CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    SelectObject(memDCImage, oldbit);
    DeleteDC(memDCImage);
}

void MakeItem(HWND hWnd, POINT p)
{
    HDC hDC;
    
    // ������ ���� �ֱ�
    item[item_num.num].num = rand() % 8 + 1;
    item[item_num.num].P.x = p.x;
    item[item_num.num].P.y = p.y;
    item[item_num.num].show = true;

    if (item_num.num == ITEM_NUM) item_num.is_end = true;
    else if(item_num.num == ITEM_NUM) item_num.is_end = false;

    if (!item_num.is_end) {
        item_num.num++;
    }
    else {
        item_num.num--;
    }
}

void ShowItem(ITEM item, HDC memDC, HDC memDCImage)
{
    // 1: ���, 2: ����, 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �𷡽ð�
    // ������ ��Ʈ�� �ε�
    if (item.num == 1)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_face.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_face_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 2)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_coin.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_coin_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 3)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_coffee.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_coffee_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 4)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_bullet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_bullet_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 5)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_lightning.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_lightning_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 6)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_shotgun.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_shotgun_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 7)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_waterwheel.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_waterwheel_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 8)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_hourglass.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_hourglass_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    // ������ ����ũ
    SelectObject(memDCImage, item_mask);
    BitBlt(memDC, item.P.x, item.P.y, 30, 30, memDCImage, 0, 0, SRCAND);

    // ������
    SelectObject(memDCImage, item_image);
    BitBlt(memDC, item.P.x, item.P.y, 30, 30, memDCImage, 0, 0, SRCPAINT);

    DeleteObject(item_image);
    DeleteObject(item_mask);
}

void CALLBACK EffectLoad(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    if (player.effect_item.effect) {
        player.effect_item.effect_cnt++;

        // ����Ʈ ���� �ε�
        if (player.effect_item.num == 3 && player.effect_item.effect_cnt == 1)
            coffee_effect = (HBITMAP)LoadImage(g_hInst, TEXT("coffee_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        else if (player.effect_item.num == 3 && player.effect_item.effect_cnt == 2)
            coffee_effect = (HBITMAP)LoadImage(g_hInst, TEXT("coffee_effect2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        else if (player.effect_item.num == 3 && player.effect_item.effect_cnt == 3)
        {
            coffee_effect = (HBITMAP)LoadImage(g_hInst, TEXT("coffee_effect3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            player.effect_item.effect_cnt = 0;
        }
        else if (player.effect_item.num == 4 && player.effect_item.effect_cnt == 1)
        {
            bullet_effect = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            bullet_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 4 && player.effect_item.effect_cnt == 2)
        {
            bullet_effect = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            bullet_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect2_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 4 && player.effect_item.effect_cnt == 3)
        {
            bullet_effect = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            bullet_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect3_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            player.effect_item.effect_cnt = 0;
        }
        else if (player.effect_item.num == 6 && player.effect_item.effect_cnt == 1)
        {
            shotgun_effect = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            shotgun_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 6 && player.effect_item.effect_cnt == 2)
        {
            shotgun_effect = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            shotgun_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect2_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 6 && player.effect_item.effect_cnt == 3)
        {
            shotgun_effect = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            shotgun_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect3_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            player.effect_item.effect_cnt = 0;
        }
        else if (player.effect_item.num == 7 && player.effect_item.effect_cnt == 1)
        {
            waterwheel_effect = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            waterwheel_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 7 && player.effect_item.effect_cnt == 2)
        {
            waterwheel_effect = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            waterwheel_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect2_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        }
        else if (player.effect_item.num == 7 && player.effect_item.effect_cnt == 3)
        {
            waterwheel_effect = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            waterwheel_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect3_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            player.effect_item.effect_cnt = 0;
        }
    }

    InvalidateRect(hWnd, NULL, false);
}

void EffectBitmapLoad()
{
    // ����Ʈ ��Ʈ�� �ε�
    coffee_effect = (HBITMAP)LoadImage(g_hInst, TEXT("coffee_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    bullet_effect = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    bullet_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("bullet_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    lightning_effect = (HBITMAP)LoadImage(g_hInst, TEXT("lightning_effect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    lightning_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("lightning_effect_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    lightning_effect_background = (HBITMAP)LoadImage(g_hInst, TEXT("lightning_effect_background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    shotgun_effect = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    shotgun_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("shotgun_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    waterwheel_effect = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    waterwheel_effect_mask = (HBITMAP)LoadImage(g_hInst, TEXT("waterwheel_effect1_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    hourglass_effect = (HBITMAP)LoadImage(g_hInst, TEXT("hourglass_effect.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void ShowEffect(ITEM item, HDC memDC, HDC memDCImage)
{
    if (item.num == 3)
    {
        // Ŀ�� ����Ʈ
        SelectObject(memDCImage, coffee_effect);
        BitBlt(memDC, 0, 0, 480, 480, memDCImage, 0, 0, SRCAND);
    }
    else if (item.num == 4)
    {
        // źâ ����Ʈ
        SelectObject(memDCImage, bullet_effect_mask);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);

        SelectObject(memDCImage, bullet_effect);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);
    }
    else if (item.num == 5)
    {
        // ���� ����Ʈ
        SelectObject(memDCImage, lightning_effect_background);
        BitBlt(memDC, 0, 0, 480, 480, memDCImage, 0, 0, SRCPAINT);

        SelectObject(memDCImage, lightning_effect_mask);
        BitBlt(memDC, player.P.x, 0, 30, player.P.y, memDCImage, 0, 0, SRCAND);

        SelectObject(memDCImage, lightning_effect);
        BitBlt(memDC, player.P.x, 0, 30, player.P.y, memDCImage, 0, 0, SRCPAINT);
    }
    else if (item.num == 6)
    {
        // ���� ����Ʈ
        SelectObject(memDCImage, shotgun_effect_mask);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);

        SelectObject(memDCImage, shotgun_effect);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);
    }
    else if (item.num == 7)
    {
        // �������� ����Ʈ
        SelectObject(memDCImage, waterwheel_effect_mask);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);

        SelectObject(memDCImage, waterwheel_effect);
        BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);
    }
    else if (item.num == 8)
    {
        // �ð� ����Ʈ
        SelectObject(memDCImage, hourglass_effect);
        BitBlt(memDC, 0, 0, 480, 480, memDCImage, 0, 0, SRCPAINT);
    }
}


void GameUI(PLAYER player, HDC hdc)
{
    HPEN hpen, oldpen;
    HBITMAP oldBitmap;
    HBRUSH hbrush, oldbrush;
    HDC memdc = CreateCompatibleDC(hdc);

    // �ð� ���
    // ���� �簢�� �����
    RECT time_rt = { 150, 10,  630, 30 };
    hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
    FillRect(hdc, &time_rt, hbrush);
    SelectObject(hdc, oldbrush);

    time_rt = { 150, 10,  630 - (play_time * 8), 30 };
    hbrush = CreateSolidBrush(RGB(0, 255, 100));
    oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
    FillRect(hdc, &time_rt, hbrush);
    SelectObject(hdc, oldbrush);
    DeleteObject(hbrush);

    // �÷��̾��� ���� ����� ���ڿ��� ��ȯ
    TCHAR livesText[20];
    wsprintf(livesText, L"Life : %3d", player.life);

    // ���� ���� ���� ������ ���ڿ��� ��ȯ
    TCHAR coinText[20];
    wsprintf(coinText, L"Coin : %3d", player.coin);

    // ���� UI ����
    TextOut(hdc, 60, 125, livesText, lstrlen(livesText)); // ���� ��ܿ� �÷��̾��� ���� ��� ���
    TextOut(hdc, 60, 165, coinText, lstrlen(coinText)); // ���� ��� �Ʒ��� ���� ���� ���� ���� ���

    // ������ UI ���
    if (player.item)
    {
        RECT haveitem_rt = { 60, 50, 110, 100 };
        hbrush = CreateSolidBrush(RGB(255, 255, 255));
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
        FillRect(hdc, &haveitem_rt, hbrush);
        SelectObject(hdc, oldbrush);

        GameUIItem(item[player.item_idx], hdc);
    }
    else
    {
        RECT haveitem_rt = { 60, 50, 110, 100 };
        hbrush = CreateSolidBrush(RGB(255, 255, 255));
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
        FillRect(hdc, &haveitem_rt, hbrush);
        SelectObject(hdc, oldbrush);
    }
    DeleteObject(hbrush);

    // �÷��̾� ��� ��Ʈ�� �ε�
    HBITMAP livesBitmap = (HBITMAP)LoadImage(NULL, L"item_face.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    oldBitmap = (HBITMAP)SelectObject(memdc, livesBitmap);
    BitBlt(hdc, 25, 115, 55, 145, memdc, 0, 0, SRCCOPY);

    // ���� ��Ʈ�� �ε�
    HBITMAP coinBitmap = (HBITMAP)LoadImage(NULL, L"item_coin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(memdc, coinBitmap);
    BitBlt(hdc, 25, 155, 55, 185, memdc, 0, 0, SRCCOPY);

    SelectObject(hdc, oldBitmap);
    DeleteObject(livesBitmap);
    DeleteObject(coinBitmap);
    DeleteObject(memdc);
}

void GameUIItem(ITEM item, HDC memDC)
{
    HDC memDCImage = CreateCompatibleDC(memDC);

    // 3: Ŀ��, 4: źâ, 5: ����, 6: ����, 7: ��������, 8: �𷡽ð�
    // ������ ��Ʈ�� �ε�
    if (item.num == 3)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_coffee.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_coffee_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 4)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_bullet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_bullet_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 5)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_lightning.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_lightning_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 6)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_shotgun.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_shotgun_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 7)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_waterwheel.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_waterwheel_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
    else if (item.num == 8)
    {
        item_image = (HBITMAP)LoadImage(g_hInst, TEXT("item_hourglass.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        item_mask = (HBITMAP)LoadImage(g_hInst, TEXT("item_hourglass_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    // ������ ����ũ
    SelectObject(memDCImage, item_mask);
    BitBlt(memDC, 70, 60, 100, 90, memDCImage, 0, 0, SRCAND);

    // ������
    SelectObject(memDCImage, item_image);
    BitBlt(memDC, 70, 60, 100, 90, memDCImage, 0, 0, SRCPAINT);

    DeleteObject(item_image);
    DeleteObject(item_mask);
    DeleteObject(memDCImage);
}


void DrawBitmap(HDC hDC, RECT rt)
{
    HDC memDC, memDCImage;
    HBRUSH hBrush, oldBrush;
    HBITMAP hbit, oldbit[2];
    int spriteX, spriteY;
    int startX = 130;
    int startY = 40;

    LoadAllBitmap();
    // ���� ���۸��� ���� �� ���� �޸� DC ����
    memDC = CreateCompatibleDC(hDC);
    memDCImage = CreateCompatibleDC(memDC);
    // sprite sheet ��ġ
    spriteX = (frame_numW % COWBOY_SPRITE_WCNT) * CELL_SIZE;
    spriteY = frame_numL * CELL_SIZE;

    // hDC�� hbit ����
    hbit = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
    // memDC hbit��ü ����
    oldbit[0] = (HBITMAP)SelectObject(memDC, hbit);

    // ���
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int cellX = col * CELL_SIZE;
            int cellY = row * CELL_SIZE;
            // �� ĭ�� ��Ʈ�� �׸���
            oldbit[1] = (HBITMAP)SelectObject(memDCImage, hCellBitmap);
            BitBlt(memDC, cellX, cellY, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCCOPY);
        }
    }

    // ���� ���
    if (currentStage == 5)
    {
        RECT river = { 0 * CELL_SIZE, 6 * CELL_SIZE , 16 * CELL_SIZE , 7 * CELL_SIZE };
        // ����
        hBrush = CreateSolidBrush(RGB(0, 100, 255));
        oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
        FillRect(memDC, &river, hBrush);
        SelectObject(memDC, oldBrush);
        DeleteObject(hBrush);

        DrawBoss(boss, memDC);
        DrawBossSkill(memDC);
    }

    // ��ֹ� ���
    if (obstacles) {
        for (int i = 0; i < obstacle_num; ++i) {
            if (obstacles[i].kind == WALL_ID) {
                DrawOneObstacle(wall_wood_block, wood_block_mask, obstacles[i], memDC, rt);
            }
            else if(obstacles[i].kind == WOOD_ID){
                DrawOneObstacle(wood_block, wood_block_mask, obstacles[i], memDC, rt);

            }
            else {
                DrawOneObstacle(move_obstacle, move_obstacle_mask, obstacles[i], memDC, rt);
            }
        }
    }

    // ������
    for (int i = 0; i < item_num.num; ++i) {
        if (item[i].show)
            ShowItem(item[i], memDC, memDCImage);
    }

    // ����Ʈ ���
    if (player.effect_item.effect)
        ShowEffect(player.effect_item, memDC, memDCImage);

    // �Ѿ� ���
    hBrush = CreateSolidBrush(RGB(0, 0, 0));
    oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
    for (int i = 0; i < bullet_num; i++)
        Ellipse(memDC, bullet[i].P.x, bullet[i].P.y, bullet[i].P.x + 5, bullet[i].P.y + 5);

    SelectObject(memDC, oldBrush);
    DeleteObject(hBrush);

    // ���� ���
    if (monster_head) {
        MONSTERNODE* p = monster_head;
        while (p->next != NULL) {
            switch (p->data.name[0]) {
            case 'N':
                DrawOneMonster(p->data, normal_monster, normal_monster_mask, memDC, rt);
                break;
            case 'R':
                DrawOneMonster(p->data, respawn_monster, respawn_monster_mask, memDC, rt);
                break;
            case 'T':
                DrawOneMonster(p->data, tank_monster, tank_monster_mask, memDC, rt);
                break;
            case 'O':
                if (!p->data.skill)
                    DrawOneMonster(p->data, obstacle_monster, obstacle_monster_mask, memDC, rt);
                else
                    DrawOneObstacleMonster(p->data, obstacle_monster, obstacle_monster_mask, memDC, rt);
                break;
            case 'B':
                DrawOneMonster(p->data, boomer_monster, boomer_monster_mask, memDC, rt);
                break;
            default:
                break;
            }
            p = p->next;
        }
        switch (p->data.name[0]) {
        case 'N':
            DrawOneMonster(p->data, normal_monster, normal_monster_mask, memDC, rt);
            break;
        case 'R':
            DrawOneMonster(p->data, respawn_monster, respawn_monster_mask, memDC, rt);
            break;
        case 'T':
            DrawOneMonster(p->data, tank_monster, tank_monster_mask, memDC, rt);
            break;
        case 'O':
            if (!p->data.skill)
                DrawOneMonster(p->data, obstacle_monster, obstacle_monster_mask, memDC, rt);
            else
                DrawOneObstacleMonster(p->data, obstacle_monster, obstacle_monster_mask, memDC, rt);
            break;
        case 'B':
            DrawOneMonster(p->data, boomer_monster, boomer_monster_mask, memDC, rt);
            break;
        default:
            break;
        }
    }

    // ��Ȱ ���
    if (respawn_head) {
        MONSTERNODE* p = respawn_head;
        while (p->next != NULL) {
            DrawRespawnMonster(p->data, respawn_monster, respawn_monster_mask, memDC, rt);
            p = p->next;
        }
        DrawRespawnMonster(p->data, respawn_monster, respawn_monster_mask, memDC, rt);
    }

    // ��ź ���
    if (bomb_head) {
        BOMBNODE* p = bomb_head;
        while (p->next != NULL) {
            DrawOneBombAndDead(p->data, bomb, bomb_mask, memDC, rt, BOMB_SPRITE_CNT);
            p = p->next;
        }
        DrawOneBombAndDead(p->data, bomb, bomb_mask, memDC, rt, BOMB_SPRITE_CNT);
    }

    // ��� ����Ʈ ���
    if (dead_head) {
        BOMBNODE* p = dead_head;
        while (p->next != NULL) {
            DrawOneBombAndDead(p->data, dead_effect, dead_effect_mask, memDC, rt, DEAD_SPRITE_CNT);
            p = p->next;
        }
        DrawOneBombAndDead(p->data, dead_effect, dead_effect_mask, memDC, rt, DEAD_SPRITE_CNT);
    }

    // ���� ���
    if (bombing_head) {
        BOMBNODE* p = bombing_head;
        while (p->next != NULL) {
            DrawOneBombing(p->data, bombing, bombing_mask, memDC, rt);
            p = p->next;
        }
        DrawOneBombing(p->data, bombing, bombing_mask, memDC, rt);
    }

    if (isEntering) {
        DrawMerchant(memDC);
        ShopItem(memDC, memDCImage);
        ShopUI(memDC);
    }

    // �÷��̾� ����ũ
    SelectObject(memDCImage, cowboy_mask);
    BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCAND);
    // �÷��̾�
    SelectObject(memDCImage, cowboy);
    BitBlt(memDC, player.P.x, player.P.y, CELL_SIZE, CELL_SIZE, memDCImage, spriteX, spriteY, SRCPAINT);

    
    // hDC�� memDC ���(����ȭ�� ���)
    BitBlt(hDC, startX, startY, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldbit[0]);
    SelectObject(memDCImage, oldbit[1]);
    DeleteObject(hbit);
    UnLoadAllBitmap();
    DeleteDC(memDC);
    DeleteDC(memDCImage);
}
