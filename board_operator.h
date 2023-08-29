#pragma once
#define DELTAS_CHESS_BOARD_OPERATOR_H

#include <optional>

#include "board.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class StandardBoardOperator {
public:
    StandardBoardOperator(Board* p_board);
    Piece execute_move(BoardCoor selection, PieceMove move);

private:
    Board* _p_board;
};
NAMESPACE_DDDELTA_END