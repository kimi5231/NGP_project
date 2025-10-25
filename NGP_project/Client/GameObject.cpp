#include "pch.h"
#include "GameObject.h"
//
//HBITMAP GameObject::_bitmap = nullptr;
//HBITMAP GameObject::_bitmapMask = nullptr;
//Vertex GameObject::_spriteCnt{};


GameObject::GameObject()
{
    Init();
}

GameObject::~GameObject()
{
    if (_bitmap) {
        DeleteObject(_bitmap);
    }
    if (_bitmapMask) {
        DeleteObject(_bitmapMask);
    }
}

void GameObject::Init()
{
}

void GameObject::Render(HDC hDC, HDC srcDC)
{
    HBITMAP oldbit;
    int cnt{};
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCPAINT);
    
    SelectObject(srcDC, oldbit);
}


void GameObject::ResetCurFrame()
{
    _curFrame.x = 0;
}

