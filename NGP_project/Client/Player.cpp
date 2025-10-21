#include "pch.h"
#include "Player.h"
#include "framework.h"

#define CELL_SIZE 10 // 나중에 수정

Player::Player(HINSTANCE hInst)
{
	_bitmap = (HBITMAP)LoadImage(hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	_bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void Player::Render(HDC hDC)
{
    HDC memDC, memDCImage;
    HBRUSH hBrush, oldBrush;
    HBITMAP hbit, oldbit[2];
    int spriteX, spriteY;
    int startX = 130;
    int startY = 40;

    // 더블 버퍼링을 위해 두 개의 메모리 DC 생성
    memDC = CreateCompatibleDC(hDC);
    memDCImage = CreateCompatibleDC(memDC);

    // hDC와 hbit 연결
    hbit = CreateCompatibleBitmap(hDC, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    // memDC hbit객체 선택
    oldbit[0] = (HBITMAP)SelectObject(memDC, hbit);

    // 플레이어 마스크
    oldbit[1] = (HBITMAP)SelectObject(memDCImage, _bitmapMask);
    BitBlt(hDC, 100, 100, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCAND);
    // 플레이어
    SelectObject(memDCImage, _bitmap);
    BitBlt(hDC, 100, 100, CELL_SIZE, CELL_SIZE, memDCImage, 0, 0, SRCPAINT);

    // hDC에 memDC 출력(최종화면 출력)
    BitBlt(hDC, startX, startY, CELL_SIZE,  CELL_SIZE, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldbit[0]);
    SelectObject(memDCImage, oldbit[1]);
    DeleteObject(hbit);
    DeleteDC(memDC);
    DeleteDC(memDCImage);
}
