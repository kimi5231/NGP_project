#include "pch.h"
#include "BombObject.h"
#include "Global.h"

BombObject::BombObject()
    : GameObject(new Bomb)
{
    if (!_bitmap || !_bitmapMask)
    {
        _bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _bitmapMask = (HBITMAP)LoadImage(hInst, (g_resourcePath / "bomb_mask.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    }

    if (!_spriteCnt.x) {
        _spriteCnt = { 6, 1 };
    }
}
