#include "pch.h"
#include "Projectile.h"
#include "StateMachine.h"

#define BULLET_SIZE 5

Projectile::Projectile(Dir dir, const Vertex& pos)
	: GameObject(new MoveState)
{
	_dir = dir;
	_status._speed = 1;
	_pos = pos;
	_type = ObjectType::Bullet;
}

void Projectile::Update()
{
	_stateMachine->Update();
}

void Projectile::Move()
{
	switch (_dir)
	{
	case Dir::Left:
		_pos.x -= _status._speed;
		break;
	case Dir::Right:
		_pos.x += _status._speed;
		break;
	case Dir::Up:
		_pos.y -= _status._speed;
		break;
	case Dir::Down:
		_pos.y += _status._speed;
		break;
	case Dir::LeftUp:
		_pos.x -= _status._speed;
		_pos.y -= _status._speed;
		break;
	case Dir::LeftDown:
		_pos.x -= _status._speed;
		_pos.y += _status._speed;
		break;
	case Dir::RightUp:
		_pos.x += _status._speed;
		_pos.y -= _status._speed;
		break;
	case Dir::RightDown:
		_pos.x += _status._speed;
		_pos.y += _status._speed;
		break;
	}
}