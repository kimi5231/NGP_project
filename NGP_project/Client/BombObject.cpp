#include "pch.h"
#include "BombObject.h"
#include "Global.h"
#include "Constant.h"

BombObject::BombObject()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"bomb.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 6, 1 };
    _type = ObjectType::Bomb;
}

BombObject::BombObject(Vertex pos)
{
    _pos = pos;
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 6, 1 };
    _type = ObjectType::Bomb;
}

void BombObject::Update()
{
    if (_timer % (BOMB_TIME / _spriteCnt.x) == 0) {
        _curFrame.x++;
    }

    if (_curFrame.x >= _spriteCnt.x) {
        if (!_isBomb) {
            DeleteObject(_bitmap);
            DeleteObject(_bitmapMask);
            _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_effect.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_effect_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            _spriteCnt = { 7, 1 };
            _curFrame = {};
            _size.x *= 3;
            _size.y *= 3;
        }
    }
}

void BombObject::Move()
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

    _pos.x = std::clamp(_pos.x, (float)(gBackgroundRect.left), (float)(gBackgroundRect.right));
    _pos.y = std::clamp(_pos.y, (float)(gBackgroundRect.top), (float)(gBackgroundRect.bottom));
}
