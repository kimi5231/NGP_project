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

    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
}

void Player::Update()
{
    // 충돌 처리
}

void Player::BuyItem(ShopItemType itemType)
{
    _currentItem = itemType;

    switch (itemType)
    {
    case ShopItemType::Shose:
        _speed += 2;
        break;
    case ShopItemType::Bullet:
        _attack += 5;
        break;
    case ShopItemType::Gun:
        _bulletSpeed += 5;
        break;
    default:
        break;
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
