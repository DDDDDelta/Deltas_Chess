#pragma once
#ifndef DELTAS_CHESS_CHESS_GAME_H
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>

#include "pieces.h"
#include "player.h"
#include "board.h"
#include "game_record.h"

NAMESPACE_DDDELTA_START
class ChessGame {
public:
    ChessGame(Player pwhite, Player pblack);
    ChessGame(GameRecord&& record);

    [[nodiscard]] inline
    E_Result get_result() const { return this->_res; };
    [[nodiscard]] inline
    const ChessBoard& get_board() const { return this->_board.get_board(); }
    const Player player_white;
    const Player player_black;

private:
    Board _board;
    bool _white_king_move;
    bool _black_king_move;
    E_Result _res;
};
NAMESPACE_DDDELTA_END

#endif //DELTAS_CHESS_CHESS_GAME_H
