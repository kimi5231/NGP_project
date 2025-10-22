#include "pch.h"
#include "GameObject.h"


#define CELL_SIZE 1000 // 나중에 수정

GameObject::GameObject(HINSTANCE hInst)
{
    _bitmap = (HBITMAP)LoadImage(hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void GameObject::Render(HDC hDC)
{
    HDC memDC, memDCImage;
    HBITMAP hbit, oldbit;

    int startX = 130;
    int startY = 40;

    // 더블 버퍼링을 위해 두 개의 메모리 DC 생성
    memDC = CreateCompatibleDC(hDC);
    memDCImage = CreateCompatibleDC(memDC);

    // hDC와 hbit 연결
    hbit = CreateCompatibleBitmap(hDC, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    // memDC hbit객체 선택
    oldbit = (HBITMAP)SelectObject(memDC, hbit);

    //RenderMask(memDC, memDCImage);
    RenderOriginal(memDC, memDCImage);

    // hDC에 memDC 출력(최종화면 출력)
    BitBlt(hDC, startX, startY, CELL_SIZE, CELL_SIZE, memDC, 0, 0, SRCCOPY);

    DeleteObject(hbit);
    DeleteDC(memDC);
    DeleteDC(memDCImage);
}

void GameObject::RenderMask(HDC hDC, HDC srcDC)
{
    HBITMAP oldbit;
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
    BitBlt(hDC, 100, 100, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCAND);
    SelectObject(srcDC, oldbit);
}

void GameObject::RenderOriginal(HDC hDC, HDC srcDC)
{
    HBITMAP oldbit;
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmap);
    BitBlt(hDC, 100, 100, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCPAINT);
    SelectObject(srcDC, oldbit);
}
