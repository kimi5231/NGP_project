#include "pch.h"
#include "GameObject.h"



GameObject::GameObject(HINSTANCE hInst)
{
    _bitmap = (HBITMAP)LoadImage(hInst, TEXT("cowboy_move.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    _bitmapMask = (HBITMAP)LoadImage(hInst, TEXT("cowboy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void GameObject::Render(HDC hDC, HDC srcDC)
{
    HBITMAP oldbit;

    int startX = 130;
    int startY = 40;
   
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCPAINT);
    
    SelectObject(srcDC, oldbit);
}
