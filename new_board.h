#pragma once
#define DELTAS_CHESS_NEW_BOARD_H

#include <array>
#include <cstdint>
#include <optional>

#include "code_utils.inc"
#include "pieces.h"
#include "moves.h"

NAMESPACE_DDDELTA_START


class Board {
public:
    using OptPiece = std::optional<Piece>;
    using OptMovement = std::optional<PossibleMovement>;
    using ChessBoard_t = std::array<std::array<OptPiece, 8>, 8>;
public:
    Board();
    OptPiece get_piece(u8 x, u8 y) const;
    OptPiece get_piece(BoardCoor coor) const;
    OptMovement get_move(u8 x, u8 y) const;
    OptMovement get_move(BoardCoor coor) const;

private:


private:
    struct KingMove : std::pair<bool, bool> {
        inline bool& operator [](E_Color color) { return color == E_Color::White ? this->first : this->second; }
    };

    ChessBoard_t _board;
    KingMove _king_move;
    i8 _last_double_pawn_move;
};

NAMESPACE_DDDELTA_END