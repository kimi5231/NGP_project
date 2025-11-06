#include "pch.h"
#include "BombObject.h"
#include "Global.h"

BombObject::BombObject()
{
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath /"bomb.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 6, 1 };

}

BombObject::BombObject(Vertex pos)
{
    _pos = pos;
    _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    _spriteCnt = { 6, 1 };
}

void BombObject::Update()
{
    _bombCount++;
    if (_bombCount % 50 == 0) {
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
            _size *= 3;
            _pos.x -= _size/2;
            _pos.y -= _size/2;
        }
        else {
            _stateMachine->ChangeState(new DeadState);
            _stateMachine->Start();
        }
    }
}

void BombObject::Render(HDC hdc, HDC srcDC)
{
    SelectObject(srcDC, _bitmapMask);
    BitBlt(hdc, _pos.x, _pos.y, _size, _size, srcDC, _curFrame.x * _size, _curFrame.y * _size, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hdc, _pos.x, _pos.y, _size, _size, srcDC, _curFrame.x * _size, _curFrame.y * _size, SRCPAINT);
}