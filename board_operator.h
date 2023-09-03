#pragma once
#define DELTAS_CHESS_BOARD_OPERATOR_H

#include <optional>

#include "new_board.h"
#include "moves.h"
#include "chess_game_interface.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class StandardBoardOperator {
public:
    explicit StandardBoardOperator(Board* p_board) : _p_board(p_board) {}
    Piece execute_move(BoardCoor selection, PieceMove piece_move, I_ChessGame* p_game);

private:
    Board* _p_board;
};
NAMESPACE_DDDELTA_END