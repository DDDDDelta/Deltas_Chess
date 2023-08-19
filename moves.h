#pragma once
#define DELTAS_CHESS_MOVES_H

#include <vector>
#include <cstdint>

#include "pieces.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
enum class E_UniqueAction : u8 {
    None,
    ShortCastle,
    LongCastle,
    EnPassant,
    DoublePawnPush,
    Promote
};


struct PieceMove {
    PieceMove(BoardCoor co, E_UniqueAction action) : coor(co), unique_action(action) {}
    BoardCoor coor;
    E_UniqueAction unique_action;
};


struct PossibleMovement {
    std::vector<PieceMove> moves;
    std::vector<PieceMove> captures;
    std::vector<PieceMove> protects;
};
NAMESPACE_DDDELTA_END
