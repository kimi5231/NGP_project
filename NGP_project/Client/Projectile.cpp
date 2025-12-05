#include "pch.h"
#include "Projectile.h"
#include "Global.h"

#define BULLET_SIZE 5

//Projectile::Projectile(Dir dir, const Vertex& pos)
//	: GameObject()
//{
//	_dir = dir;
//	_status._speed = 3;
//	_pos = pos;
//	_type = ObjectType::Bullet;
//	_size = { 10, 10 };
//}

Projectile::Projectile() : GameObject()
{
	_type = ObjectType::Bullet;
	_size = { 10, 10 };
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