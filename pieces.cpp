#include "pieces.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START


bool BoardCoor::on_board(std::int8_t delta_x, std::int8_t delta_y) const {
    if (this->x + delta_x <= 0 || this->x + delta_x > 8 ||
        this->y + delta_y <= 0 || this->y + delta_y > 8)
        return false;
    else
        return true;
}


bool BoardCoor::operator ==(const DDDelta::BoardCoor &rhs) const {
    return this->x == rhs.x && this->y == rhs.y;
}


bool PieceMove::is_valid(const DDDelta::BoardCoor& co) const {
    for (const auto& up_next : this->next) {
        auto* p_next = up_next.get();
        while (p_next) {
            if (p_next->coor == co)
                return true;
            p_next = p_next->following.get();
        }
    }
    return false;
}


PieceMove* WhitePawn_move(const BoardCoor& co) {
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


PieceMove* WhiteKnight_move(const BoardCoor& co) {
    auto ret = new PieceMove {co};

    if (co.on_board(-1, 2))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 2), nullptr)
                        ));

    if (co.on_board(-2, -1))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 2, co.y - 1), nullptr)
                        ));

    if (co.on_board(-1, 2))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 2), nullptr)
                        ));

    if (co.on_board(-2, 1))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 2, co.y + 1), nullptr)
                        ));

    if (co.on_board(2, 1))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x + 2, co.y + 1), nullptr)
                        ));

    if (co.on_board(1, 2))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 2), nullptr)
                        ));

    if (co.on_board(2, -1))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 2), nullptr)
                        ));

    if (co.on_board(1, -2))
        ret->next.emplace_back(std::move(
                std::make_unique<PieceMove::Next>(BoardCoor(co.x - 1, co.y + 2), nullptr)
                        ));

    return ret;
}



NAMESPACE_DDDELTA_END