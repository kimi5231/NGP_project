#include "pch.h"
#include "Player.h"

#define PLAYER_SPEED 3

Player::Player()
	: GameObject(new IdleState)	// 플레이어는 아이템 사용 시 외에는 State 필요 X
{
    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };

    _status._hp = 10;
    _status._speed = PLAYER_SPEED;
	_type = ObjectType::Player;
	_spriteCnt = { 8,10 };
}

void Player::Update()
{
	
}