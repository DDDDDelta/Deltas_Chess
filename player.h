#pragma once
#define DELTAS_CHESS_PLAYER_H

#include <tuple>
#include <string>
#include <cstdint>
#include <utility>

#include "code_utils.inc"

NAMESPACE_DDDELTA_START
using PlayerName = std::array<std::string, 3>;


struct Player {
    PlayerName name;
    u16 rating;
    std::string title;
};
NAMESPACE_DDDELTA_END
