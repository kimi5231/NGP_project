#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include "Global.h"
#include "ServerFramework.h"

#define DIFF 5.001  // 임시 상수

GameObject::GameObject()
    : _state{ ObjectState::Idle }
{
}

GameObject::GameObject(ObjectState state)
    : _state{ state }
{
}

GameObject::GameObject(ObjectType type, Vertex pos)
    : _type{ type }, _pos{ pos }
{
}

BoundingBox GameObject::GetBoundingBox() const
{
    return BoundingBox(_pos, _size);
}

bool GameObject::IsCollision(const GameObjectRef other) const
{
    return GetBoundingBox().Intersects(other->GetBoundingBox());
}

void GameObject::UndoPos()
{
    if (_status._speed + DIFF >= abs(_prevPos.x - _pos.x) && _status._speed + DIFF >= abs(_prevPos.y - _pos.y)) {
        _pos = _prevPos;
        g_framework->SendMovePacket(shared_from_this(), true);
    }
}

bool GameObject::IsArrive()
{
    if (_status._speed + DIFF >= abs(_targetPos.x - _pos.x) && _status._speed + DIFF >= abs(_targetPos.y - _pos.y)) {
        return true;
    }

    return false;
}

bool GameObject::SetPos(Vertex pos)
{
    _prevPos = _pos;
    _pos = pos;
    return true;
}