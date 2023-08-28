#pragma once
#define DELTAS_CHESS_NEW_BOARD_H

#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include <memory>
#include <ranges>
#include <cassert>
#include <type_traits>
#include <iostream>

#include "pieces.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
using OptPiece = std::optional<Piece>;


class Board {
public:
    using ChessBoard_t = std::array<std::array<OptPiece, 8>, 8>;

public:
    Board();

    NODISCARD inline
    OptPiece get(BoardCoor coor) const { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD inline
    OptPiece get(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; }
    NODISCARD inline
    OptPiece& get_ref(BoardCoor coor) { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD inline
    OptPiece& get_ref(i32 x, i32 y) { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; }
    NODISCARD inline
    bool is_empty(BoardCoor coor) const { assert_on_board_coor(coor); return !this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD inline
    bool is_empty(i32 x, i32 y) const { assert_on_board_xy(x, y); return !this->is_empty({ x, y }); }

    NODISCARD inline
    auto board_range() const { return this->_board | stdvw::join; }

private:
    ChessBoard_t _board;
};
NAMESPACE_DDDELTA_END
