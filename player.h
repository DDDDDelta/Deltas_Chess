#pragma once
#ifndef DELTAS_CHESS_PLAYER_H
#define DELTAS_CHESS_PLAYER_H

#include <tuple>
#include <string>
#include <cstdint>
#include <ostream>

struct Player {
    struct Name {
        std::string first_name;
        std::string last_name;
    };

    Name name;
    std::uint16_t rating;
    std::string title;
};

std::ostream& operator <<(std::ostream& os, const Player::Name& name) {
    os << name.first_name << ", " << name.last_name;
    return os;
}

#endif //DELTAS_CHESS_PLAYER_H
