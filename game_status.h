#pragma once
#define DELTAS_CHESS_GAME_STATUS_H

#include <optional>
#include <cassert>

#include "pieces.h"
#include "player.h"
#include "code_utils.inc"


NAMESPACE_DDDELTA_START
enum class E_Result : u8 {
    UNFINISHED,
    WHITE_WIN,
    BLACK_WIN,
    DRAW
};


class GameStatus {
public:
    GameStatus(Player pwhite, Player pblack);

    const Player player_white;
    const Player player_black;
    E_Result res;
};
NAMESPACE_DDDELTA_END
