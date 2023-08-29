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
using RawBoard = std::array<std::array<OptPiece, 8>, 8>;


namespace _impl {
template <E_PieceType Type>
inline constexpr std::array<std::optional<Piece>, 8> init_board_file {
    Piece(E_Color::White, Type), constant::WhitePawn, nullopt, nullopt,
    nullopt, nullopt, constant::BlackPawn, Piece(E_Color::Black, Type)
};
}


inline constexpr RawBoard standard_starting_position {
    _impl::init_board_file<E_PieceType::Rook>,
    _impl::init_board_file<E_PieceType::Knight>,
    _impl::init_board_file<E_PieceType::Bishop>,
    _impl::init_board_file<E_PieceType::Queen>,
    _impl::init_board_file<E_PieceType::King>,
    _impl::init_board_file<E_PieceType::Bishop>,
    _impl::init_board_file<E_PieceType::Knight>,
    _impl::init_board_file<E_PieceType::Rook>
};


class Board {
public:
    Board(const RawBoard& board);

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
    RawBoard _board;
};
NAMESPACE_DDDELTA_END
