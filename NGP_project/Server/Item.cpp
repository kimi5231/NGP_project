#include "pch.h"
#include "Item.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"

Item::Item()
{
	GameObject::_type = ObjectType::Item;
	std::uniform_int_distribution<> randSpawn(static_cast<int>(ItemType::Life), static_cast<int>(ItemType::Hourglass));
	_type = static_cast<ItemType>(randSpawn(gen));
}

Item::Item(Vertex pos)
	: Item()
{
	_pos = pos;
}

void Item::Update()
{
	if (_state == ObjectState::Dead) {
		g_framework->AddRemoveObject(shared_from_this());
	}
}