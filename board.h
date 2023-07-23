#pragma once
#define DELTAS_CHESS_BOARD_H

#include <tuple>
#include <array>
#include <vector>
#include <cstdint>
#include <memory>
#include <ranges>
#include <algorithm>

#include "code_utils.h"
#include "pieces.h"
#include "moves.h"

NAMESPACE_DDDELTA_START
struct PieceWithMove {
    std::optional<PossibleMovement> opt_movement;
    Piece piece;
};


using ChessBoard_t = std::array<std::array<PieceWithMove, 8>, 8>;


class Board {
public:
    Board();

    [[nodiscard]] inline
    Piece& get_piece(BoardCoor co)
        { return this->_board[co.x - 1][co.y - 1].piece; }

    [[nodiscard]] inline
    Piece& get_piece(std::uint8_t x, std::uint8_t y)
        { return this->_board[x - 1][y - 1].piece; }

    [[nodiscard]] inline
    const Piece& get_piece(BoardCoor co) const
        { return this->_board[co.x - 1][co.y - 1].piece; }

    [[nodiscard]] inline
    const Piece& get_piece(std::uint8_t x, std::uint8_t y) const
        { return this->_board[x - 1][y - 1].piece; }

    [[nodiscard]] inline
    ChessBoard_t& get_board()
        { return this->_board; }

    [[nodiscard]] inline
    const ChessBoard_t& get_board() const
        { return this->_board; }

    void set_up();
    void reset();

private:
    ChessBoard_t _board;
    std::vector<Piece> _captured_white;
    std::vector<Piece> _captured_black;
    std::uint16_t _white_lost_val;
    std::uint16_t _black_lost_val;

private:
    PossibleMovement _pawn_move(BoardCoor co) const;
    PossibleMovement _knight_move(BoardCoor co) const;
    PossibleMovement _bishop_move(BoardCoor co) const;
    PossibleMovement _rook_move(BoardCoor co) const;
    PossibleMovement _queen_move(BoardCoor co) const;
    PossibleMovement _king_move(BoardCoor co) const;

};
NAMESPACE_DDDELTA_END
