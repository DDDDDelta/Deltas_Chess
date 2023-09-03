#pragma once
#define DELTAS_CHESS_SPECIAL_CASES_H

#include <exception>

#include "new_board.h"
#include "game_status.h"
#include "chess_game_interface.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
namespace throwable {
class special_case_base {
    // friend of I_ChessGame
public:
    explicit special_case_base(I_ChessGame* p_game) : _p_game(p_game) {}
    special_case_base() = delete;
    special_case_base(const special_case_base&) = delete;
    special_case_base(special_case_base&&) = default;
    virtual ~special_case_base() noexcept;

    special_case_base& operator =(const special_case_base&) = delete;
    special_case_base& operator =(special_case_base&&) = default;

protected:
    // this function is asserted to be called only once
    bool _used_flag = false;

private:
    I_ChessGame* _p_game;
};


class pawn_promote : public special_case_base {
public:
    pawn_promote(I_ChessGame* p_game, Board* p_board, BoardCoor target, BoardCoor original);
    ~pawn_promote() override;
    // this function is asserted to be called only once
    bool select_promotion(BoardCoor selection);

private:
    Board* _p_board;
    E_Color _color;
    BoardCoor _target;
    BoardCoor _original;
};


class game_end {
public:
    game_end(BoardCoor in_check, E_Result res);
    BoardCoor king_pos;
    E_Result res;
};
} // namespace throwable
NAMESPACE_DDDELTA_END