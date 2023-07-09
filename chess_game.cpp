#include "chess_game.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START

ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _at_selection(false), _board(),
    _white_king_move(false), _black_king_move(false), _res(E_Result::UNFINISHED) {}

NAMESPACE_DDDELTA_END