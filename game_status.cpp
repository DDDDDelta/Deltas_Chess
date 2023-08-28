#include "game_status.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
GameStatus::GameStatus(Player pwhite, Player pblack) :
    player_white(std::move(pwhite)), player_black(std::move(pblack)), res(E_Result::UNFINISHED) {}
NAMESPACE_DDDELTA_END
