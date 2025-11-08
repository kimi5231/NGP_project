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
}

void Player::Update()
{
	switch (_dir) {
	case Dir::Left:
		_curFrame.y = 1;    // 시간 되면 대각선 백터로 구현
		_curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
		break;
	case Dir::Right:
		_curFrame.y = 0;
		_curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
		break;
	case Dir::Up:
		_curFrame.y = 5;
		_curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
		break;
	case Dir::Down:
		_curFrame.y = 9;
		_curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
		break;
	default:
		break;
	}

	// if 아이템 사용 이벤트 시 아래 수행
	_stateMachine->ChangeState(new UseItemState);
	_stateMachine->Start();
}