#pragma once
#ifndef DELTAS_CHESS_PIECES_H
#define DELTAS_CHESS_PIECES_H

#include <cstddef>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "code_utils.h"
#include "chess_macros.h"

NAMESPACE_DDDELTA_START


struct BoardCoor {
    std::uint8_t x;
    std::uint8_t y;

    [[nodiscard]] bool on_board(std::int8_t delta_x, std::int8_t delta_y) const;
    bool operator ==(const BoardCoor& rhs) const;
};


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


extern const Piece WhiteKing;
extern const Piece WhiteQueen;
extern const Piece WhiteRook;
extern const Piece WhiteBishop;
extern const Piece WhiteKnight;
extern const Piece WhitePawn;
extern const Piece BlackKing;
extern const Piece BlackQueen;
extern const Piece BlackRook;
extern const Piece BlackBishop;
extern const Piece BlackKnight;
extern const Piece BlackPawn;

NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_PIECES_H

