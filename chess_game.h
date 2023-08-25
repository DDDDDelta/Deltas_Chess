#pragma once
#define DELTAS_CHESS_CHESS_GAME_H

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
#include "board.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
enum class E_GameStatus : u8 {
    WAITING_TO_START,
    PAUSED,
    WHITES_TURN,
    BLACKS_TURN,
    ENDED
};


enum class E_Result : u8 {
    UNFINISHED,
    WHITE_WIN,
    BLACK_WIN,
    DRAW
};


class ChessGame {
public:
    ChessGame(Player&& pwhite, Player&& pblack);

    // observers
    NODISCARD inline E_Result get_result() const { return this->_res; }
    NODISCARD inline OptPiece get_piece(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board.get_piece(x, y); }
    NODISCARD inline OptPiece get_piece(BoardCoor co) const { assert_on_board_coor(co); return this->_board.get_piece(co); }
    NODISCARD inline
    std::optional<BoardCoor> get_selection() const {
        if (this->_selected == constant::INVALID_COOR)
            return nullopt;
        else
            return this->_selected;
    }

    const Player player_white;
    const Player player_black;

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
    E_Result _res;
    Board _board;
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
