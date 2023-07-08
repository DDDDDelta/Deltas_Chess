#pragma once
#ifndef DELTAS_CHESS_CHESS_GAME_H
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include "pieces.h"
#include "player.h"
#include "board.h"

NAMESPACE_DDDELTA_START
class ChessGame {
public:
    enum class E_Result : std::uint8_t;
    ChessGame(Player pwhite, Player pblack);
    ChessGame(GameRecord record);

private:
    Player player_white;
    Player player_black;
    Board board;
    bool white_king_move;
    bool black_king_move;
    E_Result res;

public:
    enum class E_Result : std::uint8_t {
        WHITE_WIN,
        BLACK_WIN,
        DRAW,
        UNFINISHED
    };
};
NAMESPACE_DDDELTA_END

#endif //DELTAS_CHESS_CHESS_GAME_H
