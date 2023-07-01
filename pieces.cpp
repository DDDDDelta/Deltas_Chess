#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START


PieceMove* WhitePawn::move(const BoardCoor& co) {
    auto ret = new PieceMove {co};

    if (co.x > 1 && co.y < 8)
        // take left
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 1), nullptr)
                        ));

    if (co.x < 8 && co.y < 8)
        // take right
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x + 1, co.y + 1), nullptr)
                        ));

    if (co.y < 8)
        // move forward
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x, co.y + 1), nullptr)
                        ));

    return ret;
}


PieceMove* WhiteKnight::move(const BoardCoor& co) {
    auto ret = new PieceMove {co};

    if ()

    return ret;
}


NAMESPACE_DDDELTA_END