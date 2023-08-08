#pragma once
#define DELTAS_CHESS_PIECES_H

#include <cstdint>
#include <string>
#include <vector>
#include <compare>

#include "code_utils.inc"
#include "chess_macros.h"



NAMESPACE_DDDELTA_START
struct BoardCoor {
    i8 x;
    i8 y;

    [[nodiscard]]
    bool on_board() const;

    bool operator ==(const BoardCoor& rhs) const = default;

    inline
    BoardCoor operator +(BoardCoor rhs) const;

    inline
    BoardCoor operator -(BoardCoor rhs) const;

    BoardCoor& operator +=(BoardCoor rhs);
};

using Vec2 = BoardCoor;


enum class E_Color : bool {
    White = true,
    Black = false
};


enum class E_PieceType : u8 {
    Pawn = 'P',
    Knight = 'N',
    Bishop = 'B',
    King = 'K',
    Rook = 'R',
    Queen = 'Q'
};


struct Piece {
    E_Color color;
    E_PieceType type;

    bool operator ==(const Piece&) const = default;
};


std::strong_ordering operator <=>(const Piece& lhs, const Piece& rhs);


namespace constant {
    constexpr BoardCoor INVALID_COOR{0, 0};
    constexpr Piece WhiteKing = {E_Color::White, E_PieceType::King};
    constexpr Piece WhiteQueen = {E_Color::White, E_PieceType::Queen};
    constexpr Piece WhiteRook = {E_Color::White, E_PieceType::Rook};
    constexpr Piece WhiteBishop = {E_Color::White, E_PieceType::Bishop};
    constexpr Piece WhiteKnight = {E_Color::White, E_PieceType::Knight};
    constexpr Piece WhitePawn = {E_Color::White, E_PieceType::Pawn};
    constexpr Piece BlackKing = {E_Color::Black, E_PieceType::King};
    constexpr Piece BlackQueen = {E_Color::Black, E_PieceType::Queen};
    constexpr Piece BlackRook = {E_Color::Black, E_PieceType::Rook};
    constexpr Piece BlackBishop = {E_Color::Black, E_PieceType::Bishop};
    constexpr Piece BlackKnight = {E_Color::Black, E_PieceType::Knight};
    constexpr Piece BlackPawn = {E_Color::Black, E_PieceType::Pawn};
}
NAMESPACE_DDDELTA_END

