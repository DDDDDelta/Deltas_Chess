#pragma once
#ifndef DELTAS_CHESS_MOVES_H
#define DELTAS_CHESS_MOVES_H

#include <vector>
#include <cstddef>

#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
enum class E_UniqueAction : std::uint8_t {
    None,
    ShortCastle,
    LongCastle,
    EnPassant,
    Promote
};

struct UniqueAction {
    BoardCoor coor;
    E_UniqueAction type;
};

struct PossibleMovement {
    std::vector<BoardCoor> moves;
    UniqueAction action;
};


struct RecordedMovement {
    Piece moved;
    BoardCoor original_co;
    BoardCoor final_co;
    Piece captured;
    BoardCoor captured_at;
    E_UniqueAction unique_action;
};
NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_MOVES_H
