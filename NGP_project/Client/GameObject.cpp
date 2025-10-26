#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
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

void GameObject::Update()
{
}

void GameObject::Render(HDC hdc, HDC srcDC)
{
    HBITMAP oldbit;
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
    BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCPAINT);

    SelectObject(srcDC, oldbit);
}

void GameObject::Left()
{
}

void GameObject::Right()
{
}

void GameObject::Up()
{
}

void GameObject::Down()
{
}

void GameObject::ResetCurFrame()
{
    _curFrame.x = 0;
}