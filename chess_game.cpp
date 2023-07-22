#include "chess_game.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack),_in_check(false), _board(),
    _res(E_Result::UNFINISHED), _turn(E_Color::White) {}

PossibleMovement* ChessGame::select_piece(BoardCoor co) const {
    Piece selected = this->_board.get_piece(co);

    if (selected == Empty)
        return nullptr;

    if (selected.color != this->_turn)
        return nullptr;

    return this->_board.
}

NAMESPACE_DDDELTA_END