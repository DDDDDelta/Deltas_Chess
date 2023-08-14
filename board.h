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

#include "code_utils.inc"
#include "pieces.h"
#include "moves.h"

NAMESPACE_DDDELTA_START
using OptPiece = std::optional<Piece>;


template <typename T>
struct colored_pair : std::pair<T, T> {
    inline colored_pair(T first, T second) : std::pair<T, T>(first, second) {}
    inline T& operator [](E_Color color) { return to_underlying(color) ? this->first : this->second; }
    inline const T& operator [](E_Color color) const { return to_underlying(color) ? this->first : this->second; }
};


class Board {
public:
    using ChessBoard_t = std::array<std::array<OptPiece, 8>, 8>;

private:
    using BoolPair = colored_pair<bool>;

public:
    Board();

    NODISCARD inline
    OptPiece get_piece(BoardCoor coor) const { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD inline
    OptPiece get_piece(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; };
    NODISCARD PossibleMovement* get_move(BoardCoor coor) const;
    NODISCARD PossibleMovement* get_move(i32 x, i32 y) const;
    NODISCARD inline
    BoardCoor in_check() const { return this->_in_check; }

    NODISCARD inline
    auto board_range() const { return this->_board | stdvw::join; }

private:
    ChessBoard_t _board;
    BoolPair _can_castle_short;
    BoolPair _can_castle_long;
    BoardCoor _in_check;
    BoardCoor _last_double_pawn_move;


private:
    NODISCARD PossibleMovement* _king_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _pawn_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _knight_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _bishop_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _rook_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _queen_move(BoardCoor co) const;


    PossibleMovement* _diagonal_move(PossibleMovement* p_movement, BoardCoor co) const;
    PossibleMovement* _linear_move(PossibleMovement* movement, BoardCoor co) const;
};

NAMESPACE_DDDELTA_END