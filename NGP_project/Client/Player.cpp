#include "pch.h"
#include "Player.h"

#define PLAYER_SPEED 5

Player::Player(HINSTANCE hInst)
	: GameObject(hInst)
{
    Init(hInst);
}

void Player::Init(HINSTANCE hInst)
{
    //if (!_bitmap || !_bitmapMask) {
        _bitmap = (HBITMAP)LoadImage(hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    //}
    if (!_spriteCnt.x) {
        _spriteCnt = { 8,10 };
    }
}

void Player::Left()
{
    _pos.x -= PLAYER_SPEED;
}

void Player::Right()
{
    _pos.x += PLAYER_SPEED;

}

void Player::Up()
{
    _pos.y -= PLAYER_SPEED;

}

void Player::Down()
{
    _pos.y += PLAYER_SPEED;

}
