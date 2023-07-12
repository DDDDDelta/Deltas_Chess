#include "chess_game.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _at_selection(false),_in_check(false), _board(),
    _white_king_move(false), _black_king_move(false), _res(E_Result::UNFINISHED), _turn(E_Color::White) {}

std::unique_ptr<PossibleMovement> ChessGame::select_piece(BoardCoor co) const {
    Piece selected = this->_board.get_piece(co);

    if (selected == Empty)
        return nullptr;

    if (selected.color != this->_turn)
        return nullptr;

    return this->_board.
}

NAMESPACE_DDDELTA_END