#include "pch.h"
#include "Item.h"
#include "Player.h"
#include "Global.h"
#include "TimeManager.h"
#include "Constant.h"
#include <algorithm>

Player::Player()
    : GameObject(ObjectState::Idle)
{
    if (!_bitmap || !_bitmapMask) 
    {
        _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Player.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath /"Player_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    if (!_spriteCnt.x)
        _spriteCnt = { 8,10 };

    _pos = { FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2 };

    _status._hp = 10;
    _status._speed = PLAYER_SPEED;
    _type = ObjectType::Player;
}

void Player::Update()
{
    if (_item.second) {
        if (useLightning) {   // 번개 아이템 사용 시 바로 제거
            _item.second = nullptr;
        }
        if (CheckTimer(_itemTimer, ITEM_TIME)) {
            _item.second->Expired(this);
            _item.second = nullptr;
        }
    }

    /*std::clamp(_pos.x, (int)(gBackgroundRect.left), (int)(gBackgroundRect.right));
    std::clamp(_pos.y, (int)(gBackgroundRect.top), (int)(gBackgroundRect.bottom));*/
}

void Player::UseItem()
{
    if (_item.first) {
        _item.second = _item.first;
        _item.first = nullptr;
        _item.second->ChangeState(this);
    }
}

void Player::SetItem(ItemRef item)
{
    // 목숨은 먹으면 바로 획득
    if (item->GetItemType() == ItemType::Life) {
        item->ChangeState(this);
        return;
    }
    _item.first = item;
}

void Player::Left()
{
    _pos.x -= _status._speed;
    _curFrame.y = 1;    // 시간 되면 대각선 백터로 구현
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
    SetState(ObjectState::Move);
}

void Player::Right()
{
    _pos.x += _status._speed;
    _curFrame.y = 0;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
    SetState(ObjectState::Move);
}

void Player::Up()
{
    _pos.y -= _status._speed;
    _curFrame.y = 5;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
    SetState(ObjectState::Move);
}

void Player::Down()
{
    _pos.y += _status._speed;
    _curFrame.y = 9;
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
    SetState(ObjectState::Move);
}
