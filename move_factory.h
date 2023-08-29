#pragma once
#define DELTAS_CHESS_MOVE_FACTORY_H

#include "board.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class StandardMoveFactory {
public:
    StandardMoveFactory(const Board& bd, const E_Color& _turn);
    PossibleMovement* get_move(BoardCoor selection);

private:
    const Board& _board;
    const E_Color& _turn;
};
NAMESPACE_DDDELTA_END