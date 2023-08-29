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
#include "moves.h"
#include "new_board.h"
#include "game_status.h"
#include "board_operator.h"
#include "move_factory.h"
#include "variants.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
class I_ChessGame {
    virtual const Board& board() const = 0;
    virtual const GameStatus& status() const = 0;
    virtual std::shared_ptr<const PossibleMovement> select_piece(BoardCoor co) = 0;
    virtual std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) = 0;
};


template <chess_variant V>
class NewChessGame : public I_ChessGame {
public:
    NewChessGame(const Player& p1, const Player& p2) :
        _turn(E_Color::White), _selected(constant::INVALID_COOR), _board(V::starting_position),
        _game_status(p1, p2), _board_operator(&_board), _board_factory(_board, _turn), _sp_possible_move(nullptr) {}

    NODISCARD inline const Board& board() const override { return this->_board; }
    NODISCARD inline const GameStatus& status() const override { return this->_game_status; }

    // operators
    /*
     * returns nullptr if selection is illegal
     * modifies _selected if legal
     */
    std::shared_ptr<const PossibleMovement> select_piece(BoardCoor co) override;
    /*
     * returns std::nullopt if execution is illegal
     * returns E_UniqueAction according to executed move
     * throws throwable::pawn_promote(this, target_coor) if the move is a pawn promotion
     * throws throwable::game_end if one side is checkmated or the game ended in a draw
     */
    std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) noexcept(false) override;

private:
    using Operator = V::Operator;
    using Factory = V::Factory;

private:
    E_Color _turn;
    BoardCoor _selected;
    Board _board;
    GameStatus _game_status;
    Operator _board_operator;
    Factory _board_factory;
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
