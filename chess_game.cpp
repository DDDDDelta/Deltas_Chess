#include "chess_game.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START

ChessGame::ChessGame(Player pwhite, Player pblack) :
    player_white(std::move(pwhite)), player_black(std::move(pblack)), board(),
    white_king_move(false), black_king_move(false), moves(), res(E_Result::UNFINISHED) {}

NAMESPACE_DDDELTA_END