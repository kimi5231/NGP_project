#include "pch.h"
#include "Item.h"
#include "Constant.h"

float bulletSpeed{ BULLET_TIME };
// 아이템 사용 관련 전역 변수
bool useLightning{}, useWaterWheel{}, useShotgun{}, useHourglass{};

Item::Item(ItemType type)
{
	GameObject::_type = ObjectType::Item;
	_type = type;
}

Item::Item(ItemType type, Vertex pos)
	: Item(type)
{
	_pos = pos;
}

void Item::ChangeState(GameObject* player)
{
	switch (_type) {
	case ItemType::Life:
		player->_status._life++;
		break;
	case ItemType::Magazine:
		bulletSpeed = BULLET_TIME / ADD_SPEED;
		break;
	case ItemType::Lightning:
		useLightning = true;
		break;
	case ItemType::Waterwheel:
		useWaterWheel = true;
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED + ADD_SPEED;
		break;
	case ItemType::Shotgun:
		useShotgun = true;
		break;
	case ItemType::Hourglass:
		useHourglass = true;
		break;
	}
}

void Item::Expired(GameObject* player)
{
	switch (_type) {
	case ItemType::Magazine:
		bulletSpeed = BULLET_TIME;
		break;
	case ItemType::Coffee:
		player->_status._speed = PLAYER_SPEED;
		break;
	case ItemType::Waterwheel:
		useWaterWheel = false;
		break;
	case ItemType::Shotgun:
		useShotgun = false;
		break;
	case ItemType::Hourglass:
		useHourglass = false;
		break;
	}
}