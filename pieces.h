#pragma once
#ifndef DELTAS_CHESS_PIECES_H
#define DELTAS_CHESS_PIECES_H

#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "code_utils.h"
#include "chess_macros.h"

NAMESPACE_DDDELTA_START


struct BoardCoor {
    std::uint8_t x;
    std::uint8_t y;

    [[nodiscard]] bool on_board(std::int8_t delta_x, std::int8_t delta_y) const;
    bool operator ==(const BoardCoor& rhs) const;
};


struct PieceMove {
    struct Next;
    BoardCoor curr;
    std::vector<std::unique_ptr<Next>> next;
    [[nodiscard]] bool is_valid(const BoardCoor& co) const;

    struct Next {
        BoardCoor coor;
        std::unique_ptr<Next> following;
    };
};


enum class E_Side : std::uint8_t {
    WHITE,
    BLACK,
    NEITHER
};




NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_PIECES_H

