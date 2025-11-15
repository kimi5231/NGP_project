#include "pch.h"
#include "Item.h"
#include "Player.h"
#include "Global.h"
#include "TimeManager.h"
#include "Constant.h"

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

void Player::Move(Vertex dir)
{
    _prevPos = _pos;

    // 위치 업데이트
    _pos.x += dir.x * _status._speed;
    _pos.y += dir.y * _status._speed;

    if (dir.x < 0) {
        _curFrame.y = 1;
        _dir = Dir::Left;
    }
    else if (dir.x > 0) {
        _curFrame.y = 0;
        _dir = Dir::Right;
    }
    if (dir.y < 0) {
        _curFrame.y = 5;
        _dir = Dir::Up;
    }
    else if (dir.y > 0) {
        _curFrame.y = 9;
        _dir = Dir::Down;
    }
    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}
