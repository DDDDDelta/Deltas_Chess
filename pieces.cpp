#include "pieces.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START



BoardCoor BoardCoor::operator +(BoardCoor rhs) const {
    return { static_cast<i8> (this->x + rhs.x),
             static_cast<i8> (this->y + rhs.y) };
}


BoardCoor BoardCoor::operator -(BoardCoor rhs) const {
    return { static_cast<i8> (this->x - rhs.x),
             static_cast<i8> (this->y - rhs.y) };
}

BoardCoor& BoardCoor::operator +=(BoardCoor rhs) {
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