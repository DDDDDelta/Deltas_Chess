#pragma once
#ifndef DELTAS_CHESS_MOVES_H
#define DELTAS_CHESS_MOVES_H

#include <vector>
#include <cstddef>

#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
enum class E_UniqueAction : std::uint8_t {
    ShortCastle,
    LongCastle,
    EnPassant,
    Promote,
    FirstPawnMove
};


struct PossibleMovement {
    std::vector<BoardCoor> moves;
    E_UniqueAction unique_action;
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
