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


class ChessGame {
public:
    ChessGame(Player&& pwhite, Player&& pblack);
    ChessGame(GameRecord&& record);

    // render this!
    [[nodiscard]] inline
    E_Result get_result() const
        { return this->_res; }

    [[nodiscard]] inline
    const Piece& get_piece(std::uint8_t x, std::uint8_t y) const
        { return this->_board.get_piece(x, y); }

    [[nodiscard]] inline
    const Piece& get_piece(BoardCoor co) const
        { return this->_board.get_piece(co); }

    [[nodiscard]] inline
    std::optional<BoardCoor> get_selection() const
        { return this->_selected; }

    const Player player_white;
    const Player player_black;

    // information from frontend
    PossibleMovement* select_piece(BoardCoor co) const;
    bool execute_move(BoardCoor co);

private:
    bool _in_check;
    E_Color _turn;
    std::optional<BoardCoor> _selected;
    E_Result _res;
    Board _board;
};
NAMESPACE_DDDELTA_END
