#include "pch.h"
#include "TitleScene.h"
#include "Global.h"
#include "GameObject.h"
#include "GameFramework.h"

TitleScene::TitleScene()
{
	// Create Title
	_backgroundBitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Title.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// 에디트 컨트롤의 크기 및 위치 정의
	const int EDIT_WIDTH = 250;
	const int EDIT_HEIGHT = 30;

	// 화면 중앙 계산
	int x = (FRAME_BUFFER_WIDTH - EDIT_WIDTH) / 2;
	int y = (FRAME_BUFFER_HEIGHT - EDIT_HEIGHT) / 2;

	_hEditIP = CreateWindowEx(
		WS_EX_CLIENTEDGE, L"EDIT", L"127.0.0.1",     
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER,
		x, y, EDIT_WIDTH, EDIT_HEIGHT, hWnd,
		(HMENU)100,hInst, NULL
	);

	_menuStartRt = { 150, 100, 500, 210 };
}

TitleScene::~TitleScene()
{
	if (_backgroundBitmap)
	{
		DeleteObject(_backgroundBitmap);
		_backgroundBitmap = NULL; 
	}

	if (_hEditIP)
	{
		DestroyWindow(_hEditIP);
		_hEditIP = NULL;
	}
}

void TitleScene::Update()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);

	if (PtInRect(&_menuStartRt, mousePos)) {
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			TCHAR ipAddressW[30];
			int length = GetWindowText(_hEditIP, ipAddressW, 30);
			if (length > 0)
			{
				// wchar_t -> char 변환
				char ipAddressA[30];

				WideCharToMultiByte(CP_ACP, 0, ipAddressW, -1, ipAddressA, 30, NULL, NULL);

				// char* 로 서버에 전달
				_framework->ChangeTitleToGameScene(ipAddressA);
			}
		}
	}
}

void TitleScene::Render(HDC hdc)
{
	BITMAP bmpInfo;
	GetObject(_backgroundBitmap, sizeof(BITMAP), &bmpInfo);

	HDC tempMemDC = CreateCompatibleDC(hdc);
	HBITMAP oldBmp = (HBITMAP)SelectObject(tempMemDC, _backgroundBitmap);

	// Edit 크기
	const int EDIT_WIDTH = 250;
	const int EDIT_HEIGHT = 30;

	// 화면 중앙
	int editX = (FRAME_BUFFER_WIDTH - EDIT_WIDTH) / 2;
	int editY = (FRAME_BUFFER_HEIGHT - EDIT_HEIGHT) / 2;

	// 비율 계산 
	float scaleX = (float)bmpInfo.bmWidth / FRAME_BUFFER_WIDTH;
	float scaleY = (float)bmpInfo.bmHeight / FRAME_BUFFER_HEIGHT;

	// 위쪽 배경
	StretchBlt(
		hdc,
		0, 0,
		FRAME_BUFFER_WIDTH, editY,
		tempMemDC,
		0, 0,
		(int)(FRAME_BUFFER_WIDTH * scaleX), (int)(editY * scaleY),
		SRCCOPY
	);

	// 아래쪽 배경
	StretchBlt(
		hdc,
		0, editY + EDIT_HEIGHT,
		FRAME_BUFFER_WIDTH,
		FRAME_BUFFER_HEIGHT - (editY + EDIT_HEIGHT),
		tempMemDC,
		0, (int)((editY + EDIT_HEIGHT) * scaleY),
		(int)(FRAME_BUFFER_WIDTH * scaleX),
		(int)((FRAME_BUFFER_HEIGHT - (editY + EDIT_HEIGHT)) * scaleY),
		SRCCOPY
	);
		
	// 왼쪽 배경
	StretchBlt(
		hdc,
		0, editY,
		editX, EDIT_HEIGHT,
		tempMemDC,
		0, (int)(editY * scaleY),
		(int)(editX * scaleX), (int)(EDIT_HEIGHT * scaleY),
		SRCCOPY
	);

	// 오른쪽 배경
	StretchBlt(
		hdc,
		editX + EDIT_WIDTH, editY,
		FRAME_BUFFER_WIDTH - (editX + EDIT_WIDTH),
		EDIT_HEIGHT,
		tempMemDC,
		(int)((editX + EDIT_WIDTH) * scaleX), (int)(editY * scaleY),
		(int)((FRAME_BUFFER_WIDTH - (editX + EDIT_WIDTH)) * scaleX),
		(int)(EDIT_HEIGHT * scaleY),
		SRCCOPY
	);

	SelectObject(tempMemDC, oldBmp);
	DeleteDC(tempMemDC);
}