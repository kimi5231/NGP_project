#include "pch.h"
#include "Player.h"

Player::Player(HINSTANCE hInst)
	: GameObject(hInst)
{
    init(hInst);
}

void Player::init(HINSTANCE hInst)
{
    _bitmap = (HBITMAP)LoadImage(hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}