#include "pch.h"
#include "Projectile.h"
#include "Global.h"

Projectile::Projectile(Dir dir)
{
	// 임시
	_bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Life.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	_bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Life_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	_dir = dir;
	_status._speed = 1;
}

void Projectile::Update()
{
	GameObject::Update();
}

void Projectile::Render(HDC hdc, HDC srcDC)
{
	GameObject::Render(hdc, srcDC);
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