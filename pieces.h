#pragma once
#define DELTAS_CHESS_PIECES_H

#include <cstdint>
#include <compare>

#include <QtGlobal>
#include <QHash>

#include "code_utils.inc"


NAMESPACE_DDDELTA_START
struct Vec2 {
    i32 x;
    i32 y;

    bool on_board() const;
    bool operator ==(const Vec2& rhs) const = default;
    Vec2 operator +(Vec2 rhs) const;
    Vec2 operator -(Vec2 rhs) const;
    Vec2& operator +=(Vec2 rhs);
    explicit operator bool() const;
};


using BoardCoor = Vec2;


enum class E_Color : bool {
    White = true,
    Black = false
};


inline E_Color operator !(E_Color color) {
    if (to_underlying(color))
        return E_Color::Black;
    else
        return E_Color::White;
}


enum class E_PieceType : u8 {
    Pawn   = 'P',
    Knight = 'N',
    Bishop = 'B',
    King   = 'K',
    Rook   = 'R',
    Queen  = 'Q'
};


struct Piece {
    E_Color color;
    E_PieceType type;

    bool operator ==(const Piece&) const = default;
};


std::strong_ordering operator <=>(const Piece& lhs, const Piece& rhs);


namespace constant {
inline constexpr BoardCoor INVALID_COOR { 0, 0 };
inline constexpr Piece WhiteKing   { E_Color::White, E_PieceType::King };
inline constexpr Piece WhiteQueen  { E_Color::White, E_PieceType::Queen };
inline constexpr Piece WhiteRook   { E_Color::White, E_PieceType::Rook };
inline constexpr Piece WhiteBishop { E_Color::White, E_PieceType::Bishop };
inline constexpr Piece WhiteKnight { E_Color::White, E_PieceType::Knight };
inline constexpr Piece WhitePawn   { E_Color::White, E_PieceType::Pawn };
inline constexpr Piece BlackKing   { E_Color::Black, E_PieceType::King };
inline constexpr Piece BlackQueen  { E_Color::Black, E_PieceType::Queen };
inline constexpr Piece BlackRook   { E_Color::Black, E_PieceType::Rook };
inline constexpr Piece BlackBishop { E_Color::Black, E_PieceType::Bishop };
inline constexpr Piece BlackKnight { E_Color::Black, E_PieceType::Knight };
inline constexpr Piece BlackPawn   { E_Color::Black, E_PieceType::Pawn };
}
NAMESPACE_DDDELTA_END

namespace std {

template <>
struct hash<DDDelta::Piece> {
    std::size_t operator ()(DDDelta::Piece piece) {
        return
            std::hash<DDDelta::E_Color>()(piece.color) ^
            std::hash<DDDelta::E_PieceType>()(piece.type);
    }
};

}

