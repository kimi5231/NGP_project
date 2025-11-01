#pragma once

class BoundingBox {
public:
    BoundingBox(Vertex center, Vertex size = { CELL_SIZE / 2, CELL_SIZE / 2 }) : _center{ center }, _size{size} {}
    Vertex _center{}, _size{CELL_SIZE/2, CELL_SIZE/2};

    float left() const { return _center.x - _size.x / 2; }
    float right() const { return _center.x + _size.x / 2; }
    float top() const { return _center.y - _size.y / 2; }
    float bottom() const { return _center.y + _size.y / 2; }

    bool Intersects(const BoundingBox& other) const {
        return (
            left() < other.right() &&
            right() > other.left() &&
            top() < other.bottom() &&
            bottom() > other.top()
            );
    }
};
