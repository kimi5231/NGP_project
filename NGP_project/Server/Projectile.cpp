#include "pch.h"
#include "Projectile.h"
#include "StateMachine.h"
#include "Constant.h"


Projectile::Projectile(Dir dir, const Vertex& pos)
	: GameObject()
{
	_dir = dir;
	_status._speed = 3;
	_pos = pos;
	_type = ObjectType::Bullet;
	_size = { 10, 10 };
}

void Projectile::Update()
{
	Move();
	if (_pos.x < gBackgroundRect.left || _pos.x > gBackgroundRect.right ||
		_pos.y < gBackgroundRect.top || _pos.y > gBackgroundRect.bottom) {
	}
}

bool Projectile::Move()
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

	return true;
}