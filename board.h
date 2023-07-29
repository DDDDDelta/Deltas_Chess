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

    inline
    Piece& get_piece(BoardCoor co) { return this->_board[co.x - 1][co.y - 1].piece; }

    inline
    Piece& get_piece(std::uint8_t x, std::uint8_t y) { return this->_board[x - 1][y - 1].piece; }

    inline
    const Piece& get_piece(BoardCoor co) const { return this->_board[co.x - 1][co.y - 1].piece; }

    inline
    const Piece& get_piece(std::uint8_t x, std::uint8_t y) const { return this->_board[x - 1][y - 1].piece; }

    inline
    ChessBoard_t& get_board() { return this->_board; }

    inline
    const ChessBoard_t& get_board() const { return this->_board; }

    void set_up();
    void reset();

private:
    ChessBoard_t _board;
    std::vector<Piece> _captured_white;
    std::vector<Piece> _captured_black;
    std::uint16_t _white_lost_val;
    std::uint16_t _black_lost_val;
    bool _in_check;
    bool _white_king_moved;
    bool _black_king_moved;

private:
    PossibleMovement _pawn_move(BoardCoor co) const;
    PossibleMovement _knight_move(BoardCoor co) const;
    PossibleMovement _bishop_move(BoardCoor co) const;
    PossibleMovement _rook_move(BoardCoor co) const;
    PossibleMovement _queen_move(BoardCoor co) const;
    PossibleMovement _king_move(BoardCoor co) const;

    void _diagnal_move(BoardCoor co,
                       std::vector<PieceMove>& moves,
                       std::vector<PieceMove>& captures,
                       std::vector<PieceMove>& protects) const;
    void _linear_move(BoardCoor co,
                      std::vector<PieceMove>& moves,
                      std::vector<PieceMove>& captures,
                      std::vector<PieceMove>& protects) const;

    static inline
    constexpr auto _adaptor_to_piecemove = stdvw::transform([](BoardCoor coor) -> PieceMove {
        return { coor, E_UniqueAction::None };
    });

    static inline
    constexpr auto _adaptor_leaves_empty = stdvw::filter([](BoardCoor coor) -> PieceMove {
        return { coor, E_UniqueAction::None };
    });

    static inline
    constexpr auto _adaptor_leave_on_board = stdvw::filter([](BoardCoor coor) { return coor.on_board(); });

    auto _adaptor_leave_same_color_with(E_Color color) const;
    auto _adaptor_leave_diff_color_with(E_Color color) const;
};
NAMESPACE_DDDELTA_END
