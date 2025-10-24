#include "pch.h"
#include "GameObject.h"

HBITMAP GameObject::_bitmap = nullptr;
HBITMAP GameObject::_bitmapMask = nullptr;
Vertex GameObject::_spriteCnt{};

GameObject::GameObject(HINSTANCE hInst)
{
    //init(hInst);
}

GameObject::~GameObject()
{
    if (_bitmap) {
        DeleteObject(_bitmap);
    }
    if (_bitmapMask) {
        DeleteObject(_bitmapMask);
    }
}

void GameObject::Render(HDC hDC, HDC srcDC)
{
    HBITMAP oldbit;
   
    oldbit = (HBITMAP)SelectObject(srcDC, _bitmapMask);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCAND);

    SelectObject(srcDC, _bitmap);
    BitBlt(hDC, _pos.x, _pos.y, CELL_SIZE, CELL_SIZE, srcDC, 0, 0, SRCPAINT);
    
    SelectObject(srcDC, oldbit);
}
