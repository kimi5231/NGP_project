#include "pch.h"
#include "UI.h"

void UI::Render(HDC hdc, HDC srcDC)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, _penColor);
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(_brushColor);

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    // 사각형
    RECT rect = { _box.Left(), _box.Top(), _box.Right(), _box.Bottom() };
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    if (!_text.empty()) {
        SetBkMode(hdc, TRANSPARENT); // 배경 투명
        SetTextColor(hdc, RGB(0, 0, 0)); // 글자색

        DrawText(hdc, _text.data(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // 객체 삭제
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
