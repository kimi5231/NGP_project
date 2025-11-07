#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"

GameObject::GameObject()
    : _stateMachine{new StateMachine{this}}
{
}

GameObject::GameObject(State* state)
    : _stateMachine{ new StateMachine{this, state }}
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
    BitBlt(hdc, _pos.x - _size.x / 2, _pos.y - _size.y / 2, _size.x , _size.y, srcDC, _curFrame.x * _size.x, _curFrame.y * _size.y, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hdc, _pos.x - _size.x / 2, _pos.y - _size.y / 2, _size.x, _size.y, srcDC, _curFrame.x * _size.x, _curFrame.y * _size.y, SRCPAINT);
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
    return BoundingBox(_pos, _size);
}

BoundingBox GameObject::GetTargetBoundingBox() const
{
    return BoundingBox(_targetPos);
}

bool GameObject::IsCollision(const GameObject* other) const
{
    return GetBoundingBox().Intersects(other->GetBoundingBox());
}

bool GameObject::IsClick(const POINT mouse) const
{
    return GetBoundingBox().Intersects(mouse);
}

bool GameObject::IsArrive() const
{
    return GetBoundingBox().Intersects(GetTargetBoundingBox());
}

void GameObject::ResetCurFrame()
{
    _curFrame.x = 0;
}