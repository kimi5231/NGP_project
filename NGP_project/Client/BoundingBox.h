#pragma once

class BoundingBox {
public:
    BoundingBox(Vertex center, Vertex size = { CELL_SIZE, CELL_SIZE }) : _center{ center } { _halfSize.x = size.x / 2; _halfSize.y = size.y / 2; }
    Vertex _center{}, _halfSize{};

    float Left() const { return _center.x - _halfSize.x; }
    float Right() const { return _center.x + _halfSize.x; }
    float Top() const { return _center.y - _halfSize.y; }
    float Bottom() const { return _center.y + _halfSize.y; }

    bool Intersects(const BoundingBox& other) const {
        return (
            Left() < other.Right() &&
            Right() > other.Left() &&
            Top() < other.Bottom() &&
            Bottom() > other.Top()
            );
    }

    bool Intersects(const POINT& point) const {
        return (
            Left() < point.x &&
            Right() > point.x &&
            Top() < point.y &&
            Bottom() > point.y
            );
    }

    void Render(HDC hdc, HDC srcDC)
    {
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        SelectObject(hdc, hBrush);

        Rectangle(hdc, Left(), Top(), Right(), Bottom());

        DeleteObject(hBrush);
    }
};
