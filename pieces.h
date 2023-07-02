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
    bool is_valid(const BoardCoor& co);

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


struct I_Piece {
    virtual PieceMove* move(const BoardCoor& co) = 0;
};

struct WhitePawn : public I_Piece {
    PieceMove* move(const BoardCoor& co) override;
};


struct WhiteKnight : public I_Piece {
    PieceMove* move(const BoardCoor& co) override;
};


NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_PIECES_H

