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
#include "chess_game_interface.h"
#include "special_cases.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
// template <chess_variant V>
class NewChessGame : public I_ChessGame {
    using V = variant::Standard;
public:
    NewChessGame(const Player& p1, const Player& p2) :
        _turn(E_Color::White), _selected(constant::INVALID_COOR), _board(V::starting_position),
        _game_status(p1, p2), _board_operator(&_board), _move_factory(_board, _turn), _sp_possible_move(nullptr) {}

    NODISCARD inline const Board& board() const override { return this->_board; }
    NODISCARD inline const GameStatus& status() const override { return this->_game_status; }

    // operators
    /*
     * returns std::shared_ptr bound to nullptr if selection is illegal
     * modifies _selected if legal
     */
    std::shared_ptr<const PossibleMovement> select_piece(BoardCoor co) override {
        assert(this->_selected == constant::INVALID_COOR);

        // if the selection is illegal
        if (!this->_board.get(co) || this->_board.get(co)->color != this->_turn) {
            LOG_TO_STDOUT("illegal selection");
            this->_sp_possible_move.reset();
            return this->_sp_possible_move;
        }

        LOG_TO_STDOUT("piece selected");
        this->_sp_possible_move.reset(this->_move_factory.get_move(co));
        if (this->_sp_possible_move) {
            LOG_TO_STDOUT("legal selection");
            this->_selected = co;
        }

        return this->_sp_possible_move;
    }
    /*
     * returns std::nullopt if execution is illegal
     * returns E_UniqueAction according to executed move
     * throws throwable::pawn_promote(this, target_coor) if the move is a pawn promotion
     * throws throwable::game_end if one side is checkmated or the game ended in a draw
     */
    std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) noexcept(false) override {
        assert(target_coor.on_board());
        assert(this->_selected != constant::INVALID_COOR);

        LOG_TO_STDOUT("executing move");
        auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };
        auto moves = { &this->_sp_possible_move->moves, &this->_sp_possible_move->captures }; // FIXME: unnecessary copying
        auto all_legal_move_rng = moves
            | stdvw::transform([](std::vector<PieceMove>* v) -> const std::vector<PieceMove>& { return *v; })
            | stdvw::join;
        auto it_move = stdrng::find_if(all_legal_move_rng, has_move);

        // if illegal move
        if (it_move == all_legal_move_rng.end()) {
            LOG_TO_STDOUT("illegal execution");
            this->_selected = constant::INVALID_COOR;
            return nullopt;
        }

        // may throw derived class of throwable::special_case_base and transfer the control to the thrown object
        this->_board_operator.execute_move(this->_selected, *it_move, this);

        this->_next_round();

        return it_move->unique_action;
    }

protected:
    E_Color _next_round() override {
        LOG_TO_STDOUT("next turn");
        this->_turn = !this->_turn;

        LOG_TO_STDOUT("resetting selection");
        this->_selected = constant::INVALID_COOR;
        return this->_turn;
    }

private:
    using Operator = V::Operator;
    using Factory = V::Factory;

private:
    E_Color _turn;
    BoardCoor _selected;
    Board _board;
    GameStatus _game_status;
    Operator _board_operator;
    Factory _move_factory;
    std::shared_ptr<PossibleMovement> _sp_possible_move;
};


NAMESPACE_DDDELTA_END
