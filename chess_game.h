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

#include "pieces.h"
#include "player.h"
#include "board.h"
#include "moves.h"

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


namespace throwable {
class pawn_promote;
}

class ChessGame {
public:
    ChessGame(Player&& pwhite, Player&& pblack);

    // observer
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
    NODISCARD inline BoardCoor in_check() const { return this->_board.in_check(); }


    const Player player_white;
    const Player player_black;

    // operators
    std::weak_ptr<const PossibleMovement> select_piece(BoardCoor co);
    /*
     * returns std::nullopt if execution is illegal
     * returns E_UniqueAction according to executed move
     * throws throwable::pawn_promote(this, target_coor) if the move is a pawn promotion
     * throws throwable::game_end if one side is checkmated
     */
    std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) noexcept(false);

private:
    E_Color _turn;
    BoardCoor _selected;
    E_Result _res;
    Board _board;
    std::shared_ptr<PossibleMovement> _sp_possible_move;

    friend throwable::pawn_promote;
};


namespace throwable {
class pawn_promote {
    // friend of ChessGame
public:
    pawn_promote(ChessGame* p_game, BoardCoor co);
    bool select_promotion(BoardCoor selection);

private:
    ChessGame* _p_game;
    BoardCoor _co;
    BoardCoor _original;
};


class game_end {
public:
    game_end(BoardCoor in_check, E_Result res);
    BoardCoor king_pos;
    E_Result res;
};
}
NAMESPACE_DDDELTA_END
