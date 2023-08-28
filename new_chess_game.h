#pragma once
#define DELTAS_CHESS_NEW_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>
#include <memory>
#include <optional>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <type_traits>
#include <iostream>

#include "pieces.h"
#include "player.h"
#include "new_board.h"
#include "game_status.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class NewChessGame {
public:
    NewChessGame();

    NODISCARD inline const Board& board() const { return this->_board; }
    NODISCARD inline const GameStatus& status() const { return this->_game_status; }

    // operators
    /*
     * returns std::weak_ptr bound to nullptr if selection is illegal
     * modifies _selected if legal
     */
    std::weak_ptr<const PossibleMovement> select_piece(BoardCoor co);
    /*
     * returns std::nullopt if execution is illegal
     * returns E_UniqueAction according to executed move
     * throws throwable::pawn_promote(this, target_coor) if the move is a pawn promotion
     * throws throwable::game_end if one side is checkmated or the game ended in a draw
     */
    std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) noexcept(false);

private:
    E_Color _turn;
    BoardCoor _selected;
    Board _board;
    GameStatus _game_status;
    std::shared_ptr<PossibleMovement> _sp_possible_move;
};


namespace throwable {
class pawn_promote {
    // friend of Board
public:
    pawn_promote(Board* board, BoardCoor co, BoardCoor original, E_Color* turn);
    ~pawn_promote();
    // this function is asserted to be called only once
    bool select_promotion(BoardCoor selection);

private:
    Board* _p_board;
    BoardCoor _target;
    BoardCoor _original;
    E_Color* _turn;
    bool _used_flag = false;
};


class game_end {
public:
    game_end(BoardCoor in_check, E_Result res);
    BoardCoor king_pos;
    E_Result res;
};
}
NAMESPACE_DDDELTA_END
