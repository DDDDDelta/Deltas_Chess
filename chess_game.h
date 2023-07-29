#pragma once
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>
#include <memory>
#include <optional>

#include "pieces.h"
#include "player.h"
#include "board.h"
#include "moves.h"
#include "game_record.h"

NAMESPACE_DDDELTA_START
enum class E_GameStatus : std::uint8_t {
    WAITING_TO_START,
    PAUSED,
    WHITES_TURN,
    BLACKS_TURN,
    ENDED
};

namespace throwable {
    class pawn_promote;
}

class ChessGame {
public:
    ChessGame(Player&& pwhite, Player&& pblack);

    ChessGame(GameRecord &&record); // TODO: constructor from GameRecord

    // render this!
    [[nodiscard]] inline
    E_Result get_result() const { return this->_res; }
    [[nodiscard]] inline
    const Piece &get_piece(std::uint8_t x, std::uint8_t y) const { return this->_board.get_piece(x, y); }
    [[nodiscard]] inline
    const Piece &get_piece(BoardCoor co) const { return this->_board.get_piece(co); }
    [[nodiscard]] inline
    std::optional<BoardCoor> get_selection() const {
        if (this->_selected == constant::INVALID_COOR)
            return std::nullopt;
        else
            return this->_selected;
    }

    const Player player_white;
    const Player player_black;

    // information from frontend
    const std::optional<PossibleMovement>& select_piece(BoardCoor co) const;
    std::optional<E_UniqueAction> execute_move(BoardCoor co) noexcept(false);

private:
    E_Color _turn;
    BoardCoor _selected;
    E_Result _res;
    Board _board;

    [[nodiscard]] inline
    Piece& get_piece(BoardCoor co) { return this->_board.get_piece(co); }
    [[nodiscard]] inline
    Piece& get_piece(std::uint8_t x, std::uint8_t y) { return this->_board.get_piece(x, y); }

    friend throwable::pawn_promote;
};


namespace throwable {
    class pawn_promote {
    public:
        pawn_promote(ChessGame* p_game, BoardCoor co);
        bool select_promotion(E_PieceType type);

    private:
        ChessGame* _p_game;
        BoardCoor _co;
    };
}
NAMESPACE_DDDELTA_END
