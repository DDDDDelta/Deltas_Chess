#pragma once
#define DELTAS_CHESS_PIECES_H

#include <cstddef>
#include <string>
#include <vector>
#include <compare>

#include "code_utils.h"
#include "chess_macros.h"



NAMESPACE_DDDELTA_START
struct BoardCoor {
    std::int8_t x;
    std::int8_t y;

    [[nodiscard]]
    bool on_board(std::int8_t delta_x, std::int8_t delta_y) const;

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


enum class E_Color : std::uint8_t {
    White,
    Black,
    Neither
};


inline
E_Color operator ~(E_Color color);


enum class E_PieceType : char {
    Empty = ' ',
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
    inline explicit operator bool() const;
};

auto operator <=>(const Piece& lhs, const Piece& rhs);


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
    constexpr Piece Empty = {E_Color::Neither, E_PieceType::Empty};
}
NAMESPACE_DDDELTA_END

