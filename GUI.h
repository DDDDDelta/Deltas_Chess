//
// Created by bobzheng on 2023/7/9.
//

#ifndef DELTAS_CHESS_GUI_H
#define DELTAS_CHESS_GUI_H


#include "code_utils.inc"
#include "chess_game.h"
#include "SDL2/include/SDL.h"
#include "SDL2_ttf/include/SDL_ttf.h"

NAMESPACE_BOBZHENG00_START

class GUI {
    public:
        GUI();
        ~GUI();
        void player_init(DDDelta::ChessGame chess_game, SDL_Renderer *renderer);
        void board_init(DDDelta::ChessGame chess_game);


};


NAMESPACE_BOBZHENG00_END

#endif //DELTAS_CHESS_GUI_H
