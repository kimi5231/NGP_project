#include "pch.h"
#include "Scene.h"
#include "Player.h"

void Scene::Render(HDC hdc)
{
	// 더블 버퍼링을 위해 두 개의 메모리 DC 생성
	_memDC = CreateCompatibleDC(hdc);
	_memDCImage = CreateCompatibleDC(_memDC);

	// hdc와 hbit 연결
	_hbit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	// memDC hbit객체 선택
	_oldbit = (HBITMAP)SelectObject(_memDC, _hbit);

	for (GameObjectRef object : _objects)
		object->Render(_memDC, _memDCImage);

	// hdc에 memDC 출력(최종화면 출력)
	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, _memDC, 0, 0, SRCCOPY);

	SelectObject(_memDC, _oldbit);
	DeleteObject(_hbit);
	DeleteDC(_memDC);
	DeleteDC(_memDCImage);
}