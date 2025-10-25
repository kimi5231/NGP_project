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
    switch (_state)
    {
    case ObjectState::Idle:
        Idle();
        break;
    case ObjectState::Move:
        Move();
        break;
    case ObjectState::Boom:
        Boom();
        break;
    case ObjectState::Dead:
        Dead();
        break;
    }
}

void GameObject::Render(HDC hdc, HDC srcDC)
{
    if (_type == ObjectType::Background)
    {
        BITMAP bmpInfo;
        GetObject(_bitmap, sizeof(BITMAP), &bmpInfo);

        SelectObject(srcDC, _bitmap);
        StretchBlt(hdc, _pos.x, _pos.y, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, srcDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCPAINT);
    }
    else
    {
        HBITMAP oldbit;
        int cnt{};
        oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
        BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCAND);

        SelectObject(srcDC, _bitmap);
        BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCPAINT);

        SelectObject(srcDC, oldbit);
    }
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