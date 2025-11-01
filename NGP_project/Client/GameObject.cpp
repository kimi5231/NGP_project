#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"

GameObject::GameObject()
    : _stateMachine{new StateMachine{this}}
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
    SelectObject(srcDC, _bitmapMask);
    BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hdc, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, _curFrame.x * CELL_SIZE, _curFrame.y * CELL_SIZE, SRCPAINT);
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

BoundingBox GameObject::GetBoundingBox() const
{
    return BoundingBox(_pos);
}

BoundingBox GameObject::GetTargetBoundingBox() const
{
    return BoundingBox(_targetPos);
}

bool GameObject::IsCollision(const GameObject* other) const
{
    return GetBoundingBox().Intersects(other->GetBoundingBox());
}

bool GameObject::IsArrive() const
{
    return GetBoundingBox().Intersects(GetTargetBoundingBox());
}

void GameObject::ResetCurFrame()
{
    _curFrame.x = 0;
}