#pragma once
#ifndef DELTAS_CHESS_CHESS_GAME_H
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>
#include <memory>

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

    const Player player_white;
    const Player player_black;

    // information from frontend
    std::unique_ptr<PossibleMovement> select_piece(BoardCoor co) const;
    bool execute_move(BoardCoor co);

private:
    bool _in_check;
    E_Color _turn;
    bool _white_king_move;
    bool _black_king_move;
    E_Result _res;
    Board _board;

};
NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_CHESS_GAME_H
