#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START


PieceMove* white_pawn_move(const BoardCoor& co) {
    auto ret = new PieceMove {co};

    if (co.x != 1 && co.y != 8)
        // take left
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 1), nullptr)
                        ));
    if (co.x != 8 && co.y != 8)
        // take right
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x + 1, co.y + 1), nullptr)
                        ));
    if (co.y != 8)
        // move forward
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x, co.y + 1), nullptr)
                        ));

    return ret;
}

Piece const white_pawn { E_Side::WHITE, E_PieceType::PAWN, 1, white_pawn_move };
Piece const white_knight { E_Side::WHITE, E_PieceType::KNIGHT };
Piece const white_bishop { E_Side::WHITE, E_PieceType::BISHOP };
Piece const white_rook { E_Side::WHITE, E_PieceType::ROOK };
Piece const white_queen { E_Side::WHITE, E_PieceType::QUEEN };
Piece const white_king { E_Side::WHITE, E_PieceType::KING };
Piece const black_pawn { E_Side::BLACK, E_PieceType::PAWN };
Piece const black_knight { E_Side::BLACK, E_PieceType::KNIGHT };
Piece const black_bishop { E_Side::BLACK, E_PieceType::BISHOP };
Piece const black_rook { E_Side::BLACK, E_PieceType::ROOK };
Piece const black_queen { E_Side::BLACK, E_PieceType::QUEEN };
Piece const black_king { E_Side::BLACK, E_PieceType::KING };
Piece const empty {E_Side::NEITHER, E_PieceType::EMPTY };


NAMESPACE_DDDELTA_END