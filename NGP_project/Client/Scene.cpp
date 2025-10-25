#include "pch.h"
#include "Scene.h"
#include "Player.h"

void Scene::Update()
{
	for (GameObjectRef object : _objects)
		object->Update();
}

void Scene::Render(HDC hdc)
{
	// ���� ���۸��� ���� �� ���� �޸� DC ����
	_memDC = CreateCompatibleDC(hdc);
	_memDCImage = CreateCompatibleDC(_memDC);

	// hdc�� hbit ����
	_hbit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	// memDC hbit��ü ����
	_oldbit = (HBITMAP)SelectObject(_memDC, _hbit);

	for (GameObjectRef object : _objects)
		object->Render(_memDC, _memDCImage);

	// hdc�� memDC ���(����ȭ�� ���)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, _memDC, 0, 0, SRCCOPY);

	SelectObject(_memDC, _oldbit);
	DeleteObject(_hbit);
	DeleteDC(_memDC);
	DeleteDC(_memDCImage);
}