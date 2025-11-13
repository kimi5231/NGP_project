#include "pch.h"
#include "Player.h"

#define PLAYER_SPEED 3

Player::Player()
	: GameObject(ObjectState::Idle)	// 플레이어는 아이템 사용 시 외에는 State 필요 X
{
    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };

    _status._hp = 10;
    _status._speed = PLAYER_SPEED;
	_type = ObjectType::Player;
}

void Player::Update()
{
	// 아이템 사용 시 수행
    /*if (GET_SINGLE(TimeManager)->CheckTimer(_itemTimer, ITEM_TIME)) {
        _item.second->Expired(this);
        _item.second = nullptr;
    }*/

    switch (_state) {
    case ObjectState::Idle:
        break;
    case ObjectState::Move:
        switch (_dir) {
        case Dir::Left:
            _pos.x -= _status._speed;
            break;
        case Dir::Right:
            _pos.x += _status._speed;
            break;
        case Dir::Up:
            _pos.y -= _status._speed;
            break;
        case Dir::Down:
            _pos.y += _status._speed;
            break;
        default:
            break;
        }
        break;
    case ObjectState::Dead:
        break;
    default:
        break;
    }
}

