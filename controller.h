//
// Created by bobzheng on 2023/7/20.
//

#ifndef DELTAS_CHESS_CONTROLLER_H
#define DELTAS_CHESS_CONTROLLER_H


#include <map>
#include <utility>
#include <optional>

#include "chess_game.h"

#include "SDL_image.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#include "SDL2/include/SDL.h"
#include "code_utils.inc"


NAMESPACE_BOBZHENG00_START
class Controller {
    public:
        std::optional<DDDelta::BoardCoor> opt_hover;

        Controller(GUI* gui, DDDelta::ChessGame* chess_game):
        _gui(gui), _chess_game(chess_game), _quit(false), _in_game(true)
        {}

        void run();

    private:
        GUI* _gui;
        bool _quit;
        bool _in_game;
        DDDelta::ChessGame* _chess_game;
        // std::optional<DDDelta::PossibleMovement> _wp_moves;
        std::weak_ptr<const DDDelta::PossibleMovement> _wp_moves;
        std::optional<DDDelta::BoardCoor> _opt_selected;
        std::optional<DDDelta::E_UniqueAction> _opt_executed;
        void _handle_mouse_move();
        void _handle_mouse_click();
        bool _handle_promote(SDL_Event promote, DDDelta::throwable::pawn_promote& e, DDDelta::BoardCoor coor);
        bool _handle_result(DDDelta::E_Result res);
};

NAMESPACE_BOBZHENG00_END
#endif //DELTAS_CHESS_CONTROLLER_H
