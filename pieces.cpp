#include "pieces.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
Vec2 Vec2::operator +(Vec2 rhs) const {
    return { this->x + rhs.x,
             this->y + rhs.y };
}


Vec2 Vec2::operator -(Vec2 rhs) const {
    return { this->x - rhs.x,
             this->y - rhs.y };
}


Vec2& Vec2::operator +=(Vec2 rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}


bool BoardCoor::on_board() const {
    return this->x >= 1 && this->x <= 8 && this->y >= 1 && this->y <= 8;
}


std::strong_ordering operator <=>(const Piece& lhs, const Piece& rhs) {
    if (lhs.color != rhs.color)
        return lhs.color <=> rhs.color;
    else
        return lhs.type <=> rhs.type;
}
NAMESPACE_DDDELTA_END