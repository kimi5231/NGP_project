#include "pch.h"
#include "BombObject.h"
#include "Global.h"

#define BOMB_TIMER 50

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
    _bombCount++;
    if (_bombCount % BOMB_TIMER == 0) {
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
    }
}