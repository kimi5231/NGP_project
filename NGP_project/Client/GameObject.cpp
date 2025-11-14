#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include "Global.h"

GameObject::GameObject()
    : _state{ObjectState::Idle}
{
}

GameObject::GameObject(ObjectState state)
    : _state{ state }
{
}

GameObject::GameObject(ObjectType type, Vertex pos)
    : _type{type}, _pos{ pos }
{
    if (type == ObjectType::Obstacle) {
        _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "wall_wood_block.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "wood_block_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }
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