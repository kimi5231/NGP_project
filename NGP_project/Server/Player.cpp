#include "pch.h"
#include "Player.h"
#include "Constant.h"
#include "Item.h"
#include "TimeManager.h"
#include "Global.h"
#include "ServerFramework.h"

Player::Player()
	: GameObject(ObjectState::Idle)	// 플레이어는 아이템 사용 시 외에는 State 필요 X
{
    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
    _prevPos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };

    _status._hp = 10;
    _status._speed = PLAYER_SPEED;
    _status._life = 1;
	_type = ObjectType::Player;
}

void Player::Update()
{
    // 시간 잰 다음에 풀리도록
    if (GET_SINGLE(TimeManager)->CheckTimer(_invincibleTimer, RESPAWN_TIME)) {
        _invincible = false;
    }

    if (_item.second) {
        if (GET_SINGLE(TimeManager)->CheckTimer(_itemTimer, ITEM_TIME)) {
            _item.second->Expired(this);
            _item.second = nullptr;
        }
    }
}

void Player::UseItem()
{
    if (_item.first) {
        // 사용 중이던 아이템 Expired
        if (_item.second) {
            _item.second->Expired(this);
            _item.second = nullptr;
        }

        _item.second = _item.first;
        _item.first = nullptr;
        _item.second->ChangeState(this);
    }
}

void Player::SetItem(ItemRef item)
{
    // 삭제를 나중에 해서 여러 번 먹어지는 오류 방지
    if (item->GetState() == ObjectState::Dead) return;

    ItemType itemType = item->GetItemType();
    g_framework->SendGetItemPacket(itemType, std::static_pointer_cast<Player>(shared_from_this()));
    // 목숨은 먹으면 바로 획득
    if (itemType == ItemType::Life) {
        _status._life++;
        return;
    }
    _item.first = std::make_shared<Item>(itemType);
}

void Player::Damaged(int damage)
{
    if (!_invincible) {// 무적 아닐 때 한번만
        if (_status._life <= 0) return;

        --_status._life;
        g_framework->SendSetLifePacket(std::static_pointer_cast<Player>(shared_from_this()));

        if (_status._life == 0) {
            _state = ObjectState::Dead;
            g_framework->SenUpdateObjectStatePacket(shared_from_this(), true);
            return;
        }
        _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
        _prevPos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };
        g_framework->SendMovePacket(shared_from_this(), true);
        _invincible = true; // 잠시 무적
    }
}