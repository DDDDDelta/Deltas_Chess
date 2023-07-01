#pragma once
#ifndef DELTAS_CHESS_PIECES_H
#define DELTAS_CHESS_PIECES_H

#include <cstdint>
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
};


enum class E_PieceType : std::uint8_t {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    EMPTY
};


struct PieceMove {
    struct Next;
    BoardCoor curr;
    std::vector<std::unique_ptr<Next>> next;

    struct Next {
        BoardCoor coor;
        std::unique_ptr<Next> following;
    };
};


enum class E_Side : std::uint8_t {
    WHITE,
    BLACK,
    NEITHER
};


struct Piece {
    E_Side side;
    E_PieceType type;
    std::uint8_t value;
    std::function<PieceMove* (const BoardCoor&)> move;
};


extern const Piece white_pawn;
extern const Piece white_knight;
extern const Piece white_bishop;
extern const Piece white_rook;
extern const Piece white_queen;
extern const Piece white_king;
extern const Piece black_pawn;
extern const Piece black_knight;
extern const Piece black_bishop;
extern const Piece black_rook;
extern const Piece black_queen ;
extern const Piece black_king;
extern const Piece empty;


NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_PIECES_H

