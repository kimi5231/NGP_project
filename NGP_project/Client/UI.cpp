#include "pch.h"
#include "UI.h"
#include <string>
#include "Global.h"

void UI::Render(HDC hdc, int num)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, _penColor);
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(_brushColor);

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    // 사각형
    RECT rect = { _box.Left(), _box.Top(), _box.Right(), _box.Bottom() };
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    if (!_text.empty()) 
    {
        SetBkMode(hdc, TRANSPARENT); // 배경 투명
        SetTextColor(hdc, RGB(0, 0, 0)); // 글자색

        std::wstring text;
        if (_isNumText)
            text = { _text.data() + std::to_wstring(num) };
        else
            text = { _text.data() };
        DrawText(hdc, text.data(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // 객체 삭제
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void ProgressBar::Render(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, _penColor);
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(_brushColor);

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    // 1초에 몇 픽셀?
    float pixel = _maxSize / _maxProgress;

    RECT rect;
    rect.top = _box.Top();
    rect.bottom = _box.Bottom();
    rect.left = _box._center.x - _box._halfSize.x;
    rect.right = rect.left + _progress * pixel;

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    // 객체 삭제
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void ProgressBar::Update(int serverTimer)
{
    if (serverTimer <= 0)
        serverTimer = 0;

    if (serverTimer >= _maxProgress)
        serverTimer = _maxProgress;

    _progress = serverTimer;
}

void TextureUI::Render(HDC hdc, HDC srcDC)
{
    if (!_bitmap || !_bitmapMask) return;

    // 대상 크기 (UI 박스 크기)
    POINT dstSize = { _box._halfSize.x * 2, _box._halfSize.y * 2 };

    // 마스크 먼저
    SelectObject(srcDC, _bitmapMask);
    StretchBlt(
        hdc,
        _box._center.x - _box._halfSize.x,   // 대상 X
        _box._center.y - _box._halfSize.y,   // 대상 Y
        dstSize.x,                           // 대상 너비
        dstSize.y,                           // 대상 높이
        srcDC,
        0, 0,                                // 원본 시작 좌표 (UI는 고정 이미지라면 0,0)
        CELL_SIZE,                                // 원본 너비 (CELL_SIZE)
        CELL_SIZE,                                // 원본 높이 (CELL_SIZE)
        SRCAND
    );

    // 실제 비트맵
    SelectObject(srcDC, _bitmap);
    StretchBlt(
        hdc,
        _box._center.x - _box._halfSize.x,
        _box._center.y - _box._halfSize.y,
        dstSize.x,
        dstSize.y,
        srcDC,
        0, 0,
        CELL_SIZE,                                // 원본 너비 (CELL_SIZE)
        CELL_SIZE,                                // 원본 높이 (CELL_SIZE)
        SRCPAINT
    );
}
