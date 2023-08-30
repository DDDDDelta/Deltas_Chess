#include "special_cases.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
const char* WHITE_WIN_MESSAGE = "White Wins!";
const char* BLACK_WIN_MESSAGE = "Black Wins!";
const char* DRAW_MESSAGE = "Draw";


namespace throwable {
pawn_promote::pawn_promote(Board* board, BoardCoor target, BoardCoor original, E_Color* turn) :
    _p_board(board), _target(target), _original(original), _turn(turn) {
    assert_on_board_coor(this->_original);

    LOG_TO_STDOUT("throwable constructed");
    board->_get_piece_ref(original).reset();
}


pawn_promote::~pawn_promote() {
    if (!this->_used_flag)
        this->_p_board->_get_piece_ref(this->_original).emplace(*this->_turn, E_PieceType::Pawn);
    else
        *this->_turn = !*this->_turn;
}


bool pawn_promote::select_promotion(BoardCoor selection) {
    assert(!this->_used_flag);

    LOG_TO_STDOUT("selecting promotion");
    E_PieceType type;

    if (selection.x != this->_target.x)
        return false;

    if (selection.y == nth_from_last_rank<8>[*this->_turn])
        type = E_PieceType::Queen;
    else if (selection.y == nth_from_last_rank<7>[*this->_turn])
        type = E_PieceType::Knight;
    else if (selection.y == nth_from_last_rank<6>[*this->_turn])
        type = E_PieceType::Rook;
    else if (selection.y == nth_from_last_rank<5>[*this->_turn])
        type = E_PieceType::Bishop;
    else
        return false;

    this->_p_board->_get_piece_ref(this->_target) = Piece(*this->_turn, type);
    this->_used_flag = true;
    return true;
}


game_end::game_end(BoardCoor in_check, E_Result result) : king_pos(in_check), res(result) {
    assert(this->res != E_Result::UNFINISHED);
}


const char* game_end::what() const noexcept {
    switch (this->res) {
        case E_Result::WHITE_WIN:
            return
    }
}
} // namespace throwable
NAMESPACE_DDDELTA_END