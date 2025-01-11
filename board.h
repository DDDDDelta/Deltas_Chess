#pragma once
#define DELTAS_CHESS_NEW_BOARD_H

#include <array>
#include <optional>
#include <utility>
#include <ranges>
#include <cassert>


#include "pieces.h"
#include "moves.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
using OptPiece = std::optional<Piece>;


template <typename T>
struct colored_pair : std::pair<T, T> {
    inline constexpr colored_pair(T first, T second) : std::pair<T, T>(first, second) {}
    inline constexpr T& operator [](E_Color color) { return to_underlying(color) ? this->first : this->second; }
    inline constexpr const T& operator [](E_Color color) const { return to_underlying(color) ? this->first : this->second; }
};


template <i32 Distance> requires (Distance <= 8 && Distance >= 1)
inline constexpr colored_pair<i32> nth_from_last_rank { 0 + Distance, 9 - Distance };


namespace throwable {
class pawn_promote;
}


class Board {
public:
    using ChessBoard_t = std::array<std::array<OptPiece, 8>, 8>;

private:
    using BoolPair = colored_pair<bool>;
    using CoorPair = colored_pair<BoardCoor>;

public:
    Board();

    NODISCARD inline
    OptPiece get_piece(BoardCoor coor) const { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD inline
    OptPiece get_piece(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; }
    NODISCARD PossibleMovement* get_move(BoardCoor coor) const;
    NODISCARD PossibleMovement* get_move(i32 x, i32 y) const;
    NODISCARD inline
    bool is_empty_sqr(BoardCoor coor) const { assert_on_board_coor(coor); return !this->get_piece(coor); }
    NODISCARD inline
    bool is_empty_sqr(i32 x, i32 y) const { assert_on_board_xy(x, y); return !this->get_piece(x, y); }

    NODISCARD bool is_checkmated() const;
    NODISCARD bool is_a_draw() const { return false; }

    // executes a legal move
    // returns constant::INVALID_COOR if no checks
    // returns the coordinate of king under check otherwise
    BoardCoor execute_move(BoardCoor selection, PieceMove piece_move);

    NODISCARD inline
    auto board_range() const { return this->_board | stdvws::join; }

private:
    struct CheckingCoor {
    public:
        constexpr CheckingCoor(BoardCoor attacker1 = constant::INVALID_COOR,
                               BoardCoor attacker2 = constant::INVALID_COOR) :
            _first(attacker1), _second(attacker2) {}

        CheckingCoor& operator =(CheckingCoor&&) = default;

        NODISCARD inline i32 attacker_count() const { return static_cast<bool>(this->_first) + static_cast<bool>(this->_second); }
        inline CheckingCoor& reset(BoardCoor coor1 = constant::INVALID_COOR,
                                   BoardCoor coor2 = constant::INVALID_COOR) {
            this->_first = coor1; this->_second = coor2; return *this;
        }
        NODISCARD inline BoardCoor get_first() const { return this->_first; }
        NODISCARD inline BoardCoor get_second() const { return this->_second; }

        inline CheckingCoor& add_attacker(BoardCoor coor);

    private:
        BoardCoor _first;
        BoardCoor _second;
    } _attackers;

    ChessBoard_t _board;
    BoolPair _can_castle_short;
    BoolPair _can_castle_long;
    CoorPair _king_pos;
    BoardCoor _last_double_pawn_move;

private:
    NODISCARD PossibleMovement* _king_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _pawn_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _knight_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _bishop_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _rook_move(BoardCoor co) const;
    NODISCARD PossibleMovement* _queen_move(BoardCoor co) const;

    inline OptPiece& _get_piece_ref(BoardCoor coor) { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    inline OptPiece& _get_piece_ref(i32 x, i32 y) { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; }

    PossibleMovement* _diagonal_move(PossibleMovement* p_movement, BoardCoor co) const;
    PossibleMovement* _linear_move(PossibleMovement* movement, BoardCoor co) const;

    NODISCARD bool _under_attack(BoardCoor target, E_Color color) const;

    friend throwable::pawn_promote;
};
NAMESPACE_DDDELTA_END
