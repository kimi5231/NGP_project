#include "pch.h"
#include "BombObject.h"
#include "Constant.h"

BombObject::BombObject()
{   _type = ObjectType::Bomb;
}

BombObject::BombObject(Vertex pos)
{
    _pos = pos;
    _type = ObjectType::Bomb;
}

void BombObject::Update()
{
    // 시간 지남에 따라 상태 변화하도록 변경하기
    /*if (CheckTimer(_timer, (float)BOMB_TIME / _spriteCnt.x)) {
        _curFrame.x++;
    }

    if (_curFrame.x >= _spriteCnt.x) {
        if (!_isBomb) {
            _isBomb = true;
            DeleteObject(_bitmap);
            DeleteObject(_bitmapMask);
            _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_effect.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_effect_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            _spriteCnt = { 7, 1 };
            _curFrame = {};
            _size.x *= 3;
            _size.y *= 3;
        }
        else {
            SetState(ObjectState::Dead);
        }
    }*/
}

bool BombObject::Move()
{
    _prevPos = _pos;
    switch (_dir)
    {
    case Dir::Left:
        _pos.x -= CELL_SIZE;
        break;
    case Dir::Right:
        _pos.x += CELL_SIZE;
        break;
    case Dir::Up:
        _pos.y -= CELL_SIZE;
        break;
    case Dir::Down:
        _pos.y += CELL_SIZE;
        break;
    }

    _pos.x = std::clamp(_pos.x, (int)(gBackgroundRect.left), (int)(gBackgroundRect.right));
    _pos.y = std::clamp(_pos.y, (int)(gBackgroundRect.top), (int)(gBackgroundRect.bottom));

    return true;
}
