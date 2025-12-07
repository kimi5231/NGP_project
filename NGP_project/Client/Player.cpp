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

void Player::SetItem(ItemType type)
{
    // 목숨은 먹으면 바로 획득
    if (type == ItemType::Life) {
        _status._life++;
        return;
    }
    ItemRef item = std::make_shared<Item>(type);
    _item.first = item;
}

void Player::Move(Vertex vecDir, Dir dir)
{
    SetState(ObjectState::Move);

    _prevPos = _pos;

    // 대각선 정규화
    float length = sqrt(vecDir.x * vecDir.x + vecDir.y * vecDir.y);
    float nx = vecDir.x / length;
    float ny = vecDir.y / length;
    
    // 위치 업데이트
    double delta = GET_SINGLE(TimeManager)->GetDeltaTime();
    _pos.x += nx * _status._speed * delta;
    _pos.y += ny * _status._speed * delta;

    // 방향 설정
    _dir = dir;

    switch (_dir)
    {
    case Dir::Left:        _curFrame.y = 1; break;
    case Dir::Right:       _curFrame.y = 0; break;
    case Dir::Up:          _curFrame.y = 5; break;
    case Dir::Down:        _curFrame.y = 9; break;
    case Dir::LeftUp:      _curFrame.y = 6; break;
    case Dir::LeftDown:    _curFrame.y = 8; break;
    case Dir::RightUp:     _curFrame.y = 4; break;
    case Dir::RightDown:   _curFrame.y = 2; break;
    }

    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;

    // clamp
    ClampPlayerArea();
}

void Player::SetDirAndFrame(Dir dir)
{
    switch (dir)
    {
    case Dir::Left:        _curFrame.y = 1; break;
    case Dir::Right:       _curFrame.y = 0; break;
    case Dir::Up:          _curFrame.y = 5; break;
    case Dir::Down:        _curFrame.y = 9; break;
    case Dir::LeftUp:      _curFrame.y = 6; break;
    case Dir::LeftDown:    _curFrame.y = 8; break;
    case Dir::RightUp:     _curFrame.y = 4; break;
    case Dir::RightDown:   _curFrame.y = 2; break;
    }

    _curFrame.x = (_curFrame.x + 1) % _spriteCnt.x;
}

void Player::ClampPlayerArea()
{    
    int sizeOffset = CELL_SIZE / 2;

    float minX = gBackgroundRect.left + sizeOffset + CELL_SIZE - 5;                      // 벽 오른쪽 한 칸
    float maxX = gBackgroundRect.left + sizeOffset + (BOARD_SIZE - 2) * CELL_SIZE + 5;   // 벽 왼쪽 한 칸 내부

    float minY = gBackgroundRect.top + sizeOffset + CELL_SIZE;                       // 위쪽 벽 아래 한 칸
    float maxY = gBackgroundRect.top + sizeOffset + (BOARD_SIZE - 2) * CELL_SIZE;    // 아래쪽 벽 위 한 칸

    _pos.x = std::clamp(_pos.x, minX, maxX);
    _pos.y = std::clamp(_pos.y, minY, maxY);
}
