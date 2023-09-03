#include "special_cases.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
namespace throwable {
special_case_base::~special_case_base() noexcept {
    if (this->_used_flag)
        this->_p_game->_next_round();
}

pawn_promote::pawn_promote(I_ChessGame* p_game, Board* p_board, BoardCoor target, BoardCoor original) :
    special_case_base(p_game), _p_board(p_board), _target(target),
    _original(original), _color(p_game->board().get(original)->color) {
    assert_on_board_coor(this->_original);

    p_board->get_ref(original).reset();
}


pawn_promote::~pawn_promote() {
    if (!this->_used_flag)
        this->_p_board->get_ref(this->_original).emplace(this->_color, E_PieceType::Pawn);
}


bool pawn_promote::select_promotion(BoardCoor selection) {
    assert(!this->_used_flag);

    LOG_TO_STDOUT("selecting promotion");
    E_PieceType type;

    if (selection.x != this->_target.x)
        return false;

    if (selection.y == nth_from_last_rank<8>[this->_color])
        type = E_PieceType::Queen;
    else if (selection.y == nth_from_last_rank<7>[this->_color])
        type = E_PieceType::Knight;
    else if (selection.y == nth_from_last_rank<6>[this->_color])
        type = E_PieceType::Rook;
    else if (selection.y == nth_from_last_rank<5>[this->_color])
        type = E_PieceType::Bishop;
    else
        return false;

    this->_p_board->get_ref(this->_target) = Piece(this->_color, type);
    this->_used_flag = true;
    return true;
}


game_end::game_end(BoardCoor in_check, E_Result result) : king_pos(in_check), res(result) {
    assert(this->res != E_Result::UNFINISHED);
}

/*
const char* game_end::what() const noexcept {
    switch (this->res) {
        case E_Result::WHITE_WIN:
            return;
    }
}
 */
} // namespace throwable
NAMESPACE_DDDELTA_END