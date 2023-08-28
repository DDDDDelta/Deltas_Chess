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
#include "SDL_ttf.h"
#include "SDL.h"
#include "code_utils.inc"


NAMESPACE_BOBZHENG00_START

enum class E_AfterGameDecision : u8 {
    Quit,
    Switch,
    Restart,
};


class Controller {
public:
    std::optional<DDDelta::BoardCoor> opt_hover;

    Controller(GUI* gui, DDDelta::ChessGame* chess_game):
            _gui(gui), _chess_game(chess_game), _in_game(true) {
        _opt_selected.reset();
    }

    void run();

private:
    GUI* _gui;
    bool _in_game;
    DDDelta::ChessGame* _chess_game;
    std::weak_ptr<const DDDelta::PossibleMovement> _wp_moves;
    std::optional<DDDelta::BoardCoor> _opt_selected;
    void _handle_mouse_move();
    void _handle_mouse_click();
    bool _handle_board_operation();
    bool _handle_promote(SDL_Event promote, DDDelta::throwable::pawn_promote& e);
    E_AfterGameDecision _handle_result();

//    enum _custom_event_type
//    {
//        timer = 1
//    };
};

NAMESPACE_BOBZHENG00_END
#endif //DELTAS_CHESS_CONTROLLER_H
