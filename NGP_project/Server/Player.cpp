#include "pch.h"
#include "Player.h"

#define PLAYER_SPEED 3

Player::Player()
{
    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };

    _status._hp = 10;
    _status._speed = PLAYER_SPEED;
}

void Player::Update()
{

}