#include "pch.h"
#include "Projectile.h"
#include "Global.h"
#include "StateMachine.h"

#define BULLET_SIZE 5

Projectile::Projectile(Dir dir, const Vertex& pos)
	: GameObject(new MoveState)
{
	// 임시
	_bitmap = (HBITMAP)LoadImage(hInst, L"Resource\\Bitmap\\Life.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	_bitmapMask = (HBITMAP)LoadImage(hInst, L"Resource\\Bitmap\\Life_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	_dir = dir;
	_status._speed = 1;
	_pos = pos;
	_type = ObjectType::Bullet;
}

void Projectile::Update()
{
	_stateMachine->Update();
}

void Projectile::Render(HDC hdc, HDC srcDC)
{
	HBRUSH hBrush;
	// 총알 출력
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, _pos.x, _pos.y, _pos.x + BULLET_SIZE, _pos.y + BULLET_SIZE);

	DeleteObject(hBrush);
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