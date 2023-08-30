#pragma once
#define DELTAS_CHESS_SPECIAL_CASES_H

#include <exception>

#include "board.h"
#include "game_status.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
namespace throwable {
class special_case_base {
public:
    special_case_base() = default;
    special_case_base(const special_case_base&) = delete;
    special_case_base(special_case_base&&) = default;
    virtual ~special_case_base() = default;

    special_case_base& operator =(const special_case_base&) = delete;
    special_case_base& operator =(special_case_base&&) = default;
};


class pawn_promote : public special_case_base {
    // friend of Board
public:
    pawn_promote(Board* board, BoardCoor co, BoardCoor original, E_Color* turn);
    ~pawn_promote() override;
    // this function is asserted to be called only once
    bool select_promotion(BoardCoor selection);

private:
    Board* _p_board;
    BoardCoor _target;
    BoardCoor _original;
    E_Color* _turn;
    bool _used_flag = false;
};


class game_end : public std::exception {
public:
    game_end(BoardCoor in_check, E_Result res);
    BoardCoor king_pos;
    E_Result res;
    const char* what() const noexcept override;
};
} // namespace throwable
NAMESPACE_DDDELTA_END