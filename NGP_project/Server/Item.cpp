#include "pch.h"
#include "Item.h"
#include "Constant.h"
#include "Global.h"
#include "ServerFramework.h"
#include "Room.h"
#include "Monster.h"

Item::Item()
{
	GameObject::_type = ObjectType::Item;
	std::uniform_int_distribution<> randSpawn(static_cast<int>(ItemType::Life), static_cast<int>(ItemType::Hourglass));
	_type = static_cast<ItemType>(randSpawn(gen));
}

Item::Item(ItemType item)
	: _type{item}
{
	GameObject::_type = ObjectType::Item;
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

void Item::ChangeState(GameObject* player)
{
	switch (_type) {
	case ItemType::Magazine:
		g_useMagazine = true;
		break;
	case ItemType::Lightning:
	{
		std::unordered_map<int, MonsterRef> monsters = g_framework->GetRoom()->GetMonsters();
		for (auto& monster : monsters) {
			monster.second->GetStateMachine()->ChangeState(new DeadState);
			monster.second->GetStateMachine()->Start();
		}
	}
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED * ADD_SPEED;
		break;
	case ItemType::Hourglass: 
	{
		std::unordered_map<int, MonsterRef> monsters = g_framework->GetRoom()->GetMonsters();
		for (auto& monster : monsters) {
			monster.second->GetStateMachine()->ChangeState(new IdleState);
			monster.second->GetStateMachine()->Start();
		}
	}
		break;
	}
}

void Item::Expired(GameObject* player)
{
	switch (_type) {
	case ItemType::Magazine:
		g_useMagazine = false;
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED;
		break;
	case ItemType::Hourglass:
	{
		std::unordered_map<int, MonsterRef> monsters = g_framework->GetRoom()->GetMonsters();
		for (auto& monster : monsters) {
			monster.second->GetStateMachine()->ChangeState(new MoveToTargetState);
			monster.second->GetStateMachine()->Start();
		}
	}
		break;
	}
}