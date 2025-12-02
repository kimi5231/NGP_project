#include "pch.h"
#include "UI.h"
#include <string>
#include "Global.h"

void UI::Render(HDC hdc, HDC srcDC, int num)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, _penColor);
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(_brushColor);

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    // 사각형
    RECT rect = { _box.Left(), _box.Top(), _box.Right(), _box.Bottom() };
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    // 객체 삭제
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

void ProgressBar::Update(int serverTimer)
{
    _progress = serverTimer;

    // 0이 아니면 진행
    if (_progress == 0) {
        _box._halfSize.x = 0;
        return;
    }
    
    _box._halfSize.x -= _maxSize / _maxProgress;
    _box._center.x -= _maxSize / _maxProgress;
}
