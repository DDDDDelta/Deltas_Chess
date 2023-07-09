#pragma once
#ifndef DELTAS_CHESS_BOARD_H
#define DELTAS_CHESS_BOARD_H

#include <tuple>
#include <array>
#include <vector>
#include <cstdint>
#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
using ChessBoard = std::array<std::array<Piece, 8>, 8>;


class Board {
public:
    Board();
    [[nodiscard]] Piece& get_piece(BoardCoor co);
    [[nodiscard]] const ChessBoard& get_board() const;
    void set_up();
    void reset();

private:
    ChessBoard _board;
    std::vector<Piece> _captured_white;
    std::vector<Piece> _captured_black;
    std::uint16_t _white_lost_val;
    std::uint16_t _black_lost_val;

};
NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_BOARD_H
