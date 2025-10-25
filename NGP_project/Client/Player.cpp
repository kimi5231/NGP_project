#include "pch.h"
#include "Player.h"
#include "Global.h"

#define PLAYER_SPEED 5

Player::Player()
{
    if (!_bitmap || !_bitmapMask) 
    {
        _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Player.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Player_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    if (!_spriteCnt.x)
        _spriteCnt = { 8,10 };

    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
}

void Player::Update()
{
}

void Player::Render(HDC hdc, HDC srcDC)
{
    GameObject::Render(hdc, srcDC);
}

void Player::Left()
{
    _pos.x -= PLAYER_SPEED;
    _curFrame.y = 1;    // 시간 되면 대각선 백터로 구현
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Player::Right()
{
    _pos.x += PLAYER_SPEED;
    _curFrame.y = 0;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Player::Up()
{
    _pos.y -= PLAYER_SPEED;
    _curFrame.y = 5;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;

}

void Player::Down()
{
    _pos.y += PLAYER_SPEED;
    _curFrame.y = 9;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;

}
