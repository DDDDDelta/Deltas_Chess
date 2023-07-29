#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START


bool BoardCoor::on_board(std::int8_t delta_x, std::int8_t delta_y) const {
    if (this->x + delta_x < 1 || this->x + delta_x > 8 ||
        this->y + delta_y < 1 || this->y + delta_y > 8)
        return false;
    else
        return true;
}

BoardCoor BoardCoor::operator +(BoardCoor rhs) const {
    return { static_cast<std::int8_t> (this->x + rhs.x),
             static_cast<std::int8_t> (this->y + rhs.y) };
}


BoardCoor BoardCoor::operator -(BoardCoor rhs) const {
    return { static_cast<std::int8_t> (this->x - rhs.x),
             static_cast<std::int8_t> (this->y - rhs.y) };
}

BoardCoor& BoardCoor::operator +=(BoardCoor rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

bool BoardCoor::on_board() const {
    return this->x >= 1 && this->x <= 8 && this->y >= 1 && this->y <= 8;
}

E_Color operator ~(E_Color color) {
    if (color == E_Color::White)
        return E_Color::Black;
    else if (color == E_Color::Black)
        return E_Color::White;
    else
        return color;
}

Piece::operator bool() const { return *this == Empty; }

auto operator <=>(const Piece& lhs, const Piece& rhs) {
    if (lhs.color != rhs.color)
        return lhs.color <=> rhs.color;
    else
        return lhs.type <=> rhs.type;
}


NAMESPACE_DDDELTA_END