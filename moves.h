#pragma once
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


struct PieceMove {
    BoardCoor coor;
    E_UniqueAction unique_action;
};


struct PossibleMovement {
    std::vector<PieceMove> moves;
    std::vector<PieceMove> captures;
    std::vector<PieceMove> protects;
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
