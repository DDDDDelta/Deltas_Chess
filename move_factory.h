#pragma once
#define DELTAS_CHESS_MOVE_FACTORY_H

#include "new_board.h"
#include "moves.h"
#include "game_status.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class StandardMoveFactory {
public:
    StandardMoveFactory(const Board& bd, const GameStatus& status);
    PossibleMovement* get_move(BoardCoor selection);

private:
    using BoolPair = colored_pair<bool>;
    using CoorPair = colored_pair<BoardCoor>;

private:
    BoolPair _can_castle_short;
    BoolPair _can_castle_long;
    CoorPair _king_pos;
    BoardCoor _last_double_pawn_move;

    const Board& _board;
    const GameStatus& _status;
};
NAMESPACE_DDDELTA_END