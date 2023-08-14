#pragma once
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>
#include <memory>
#include <optional>
#include <cassert>

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

    // render this!
    NODISCARD inline E_Result get_result() const { return this->_res; }
    NODISCARD inline const Piece& get_piece(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board.get_piece(x, y); }
    NODISCARD inline const Piece& get_piece(BoardCoor co) const { assert_on_board_coor(co); return this->_board.get_piece(co); }
    NODISCARD inline
    std::optional<BoardCoor> get_selection() const {
        if (this->_selected == constant::INVALID_COOR)
            return std::nullopt;
        else
            return this->_selected;
    }


    const Player player_white;
    const Player player_black;

    // operators
    const std::optional<PossibleMovement>& select_piece(BoardCoor co);
    // throws pawn_promote for pawn promotion
    std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) noexcept(false);

private:
    E_Color _turn;
    BoardCoor _selected;
    E_Result _res;
    Board _board;

    NODISCARD inline
    Piece& get_piece(BoardCoor co) { assert_on_board_coor(co); return this->_board.get_piece(co); }
    NODISCARD inline
    Piece& get_piece(i32 x, i32 y) { assert_on_board_xy(x, y); return this->_board.get_piece(x, y); }

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
}
NAMESPACE_DDDELTA_END
