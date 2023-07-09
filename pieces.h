#pragma once
#ifndef DELTAS_CHESS_PIECES_H
#define DELTAS_CHESS_PIECES_H

#include <cstddef>
#include <string>
#include <vector>

#include "code_utils.h"
#include "chess_macros.h"

NAMESPACE_DDDELTA_START
struct BoardCoor {
    std::uint8_t x;
    std::uint8_t y;

    [[nodiscard]] bool on_board(std::int8_t delta_x, std::int8_t delta_y) const;
    bool operator ==(BoardCoor rhs) const;
};


extern constexpr BoardCoor INVALID_COOR { 0 , 0 };


enum class E_PieceColor : std::uint8_t {
    White,
    Black,
    Neither
};


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
    E_PieceColor color;
    E_PieceType type;
};

constexpr Piece WhiteKing = { E_PieceColor::White, E_PieceType::King };
constexpr Piece WhiteQueen = { E_PieceColor::White, E_PieceType::Queen };
constexpr Piece WhiteRook = { E_PieceColor::White, E_PieceType::Rook };
constexpr Piece WhiteBishop = { E_PieceColor::White, E_PieceType::Bishop };
constexpr Piece WhiteKnight = { E_PieceColor::White, E_PieceType::Knight };
constexpr Piece WhitePawn = { E_PieceColor::White, E_PieceType::Pawn };
constexpr Piece BlackKing = { E_PieceColor::Black, E_PieceType::King };
constexpr Piece BlackQueen = { E_PieceColor::Black, E_PieceType::Queen };
constexpr Piece BlackRook = { E_PieceColor::Black, E_PieceType::Rook };
constexpr Piece BlackBishop = { E_PieceColor::Black, E_PieceType::Bishop };
constexpr Piece BlackKnight = { E_PieceColor::Black, E_PieceType::Knight };
constexpr Piece BlackPawn = { E_PieceColor::Black, E_PieceType::Pawn };
constexpr Piece Empty = { E_PieceColor::Neither, E_PieceType::Empty };

NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_PIECES_H

