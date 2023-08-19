//
// Created by bobzheng on 2023/7/20.
//

#include <iostream>
#include "GUI.h"
#include "moves.h"
#include "controller.h"

#include <SDL.h>

#include "code_utils.inc"

#define ON_BOARD(x, y) (x < 800 && y < 800)
#define NEED_REDRAW_PREVIOUS_TILE(mouse_x, mouse_y, board_x, board_y) \
(board_x != 0 && board_y != 0 && ((mouse_x / 100 + 1) != board_x || (mouse_y / 100 + 1) != board_y))
#define TO_INT8(x)  (static_cast<std::int8_t> (x))
#define TO_BOARDCOOR(x, y) (DDDelta::BoardCoor(TO_INT8(x / 100 + 1), TO_INT8(y / 100 + 1)))
#define ABLE_TO_SELECT(x, y) (!_chess_game->get_selection())
#define UNABLE_TO_EXECUTE(x, y) (!_chess_game->execute_move(TO_BOARDCOOR(x, y)))
#define NOT_UNIQUE_ACTION(x, y) (_chess_game->execute_move(TO_BOARDCOOR(x, y)).value() == DDDelta::E_UniqueAction::None)
#define PROMOTE_START(x, y) (_chess_game->execute_move(TO_BOARDCOOR(x, y)).value() == DDDelta::E_UniqueAction::Promote)
NAMESPACE_BOBZHENG00_START


void Controller::_handle_mouse_move() {
    if ((!_in_game) || (_opt_selected)) return;
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (ON_BOARD(mouse_x, mouse_y))
    {
        std::cout << "on board" << std::endl;
        // Redraw previous hovered tile and piece
        if (NEED_REDRAW_PREVIOUS_TILE(mouse_x, mouse_y, opt_hover->x, opt_hover->y) && opt_hover)
        {
            std::cout << "leaving" << std::endl;
            _gui->render_tile(opt_hover.value());
            _gui->render_piece(opt_hover.value());
        }

        // Update current tile
        opt_hover = TO_BOARDCOOR(mouse_x, mouse_y);

        // Draw hovered tile and piece
        _gui->set_hover(opt_hover.value());

        _gui->render_tile(opt_hover.value());
        _gui->render_piece(opt_hover.value());

        SDL_RenderPresent(_gui->renderer);

    }
}


void Controller::_handle_mouse_click() {

    if (!_in_game) return;
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);


    if (ON_BOARD(mouse_x, mouse_y))
    {
        std::cout << "click on board" << std::endl;
        if (!_opt_selected) //TODO !chess_game->get_selection()
        {
            std::cout << "selected one piece" << std::endl;
            _opt_moves = _chess_game->select_piece(TO_BOARDCOOR(mouse_x, mouse_y));
            _opt_selected = _chess_game->get_selection();

            if (_opt_moves.lock() == nullptr) {
                std::cout << "selected nothing" << std::endl;
                return;
            }

            _gui->render_select(_chess_game->get_selection().value());
            _gui->render_piece(_chess_game->get_selection().value());

//            std::cout << _opt_moves.lock()->moves[0].coor.x << std::endl;
//            std::cout << _opt_moves.lock()->moves[0].coor.y << std::endl;
            std::cout << "selection render finished" << std::endl;

            for (auto pm : _opt_moves.lock()->captures)
            {
                _gui->render_capture(pm.coor);
                _gui->render_piece(pm.coor);
            }

            for (auto pm : _opt_moves.lock()->moves)
            {
                _gui->render_move(pm.coor);
                _gui->render_piece(pm.coor);
            }

            SDL_RenderPresent(_gui->renderer);
            std::cout << "possible moves render finished" << std::endl;
            return;
        }

        if (_opt_selected) // TODO chess_game->get_selection()
        {
            std::cout << "is ready to move" << std::endl;
            std::optional<DDDelta::E_UniqueAction> action;
            try
            {
                std::cout << "check if unique" << std::endl;
                action = _chess_game->execute_move(TO_BOARDCOOR(mouse_x, mouse_y));
                std::cout << "finish execute" << std::endl;
            }
            catch(DDDelta::throwable::pawn_promote& e)
            {
                _gui->render_promote_selection(TO_BOARDCOOR(mouse_x, mouse_y));

                SDL_Event promote;
                while (true)
                {
                    SDL_WaitEvent(&promote);
                    if (_handle_promote(promote, e, TO_BOARDCOOR(mouse_x, mouse_y))) break;
                }

                return;
            }

            if (!action)
            {
                _gui->render_tile(_opt_selected.value());
                _gui->render_piece(_opt_selected.value());

                for (auto pm : _opt_moves.lock()->captures)
                {
                    _gui->render_tile(pm.coor);
                    _gui->render_piece(pm.coor);
                }

                for (auto pm : _opt_moves.lock()->moves)
                {
                    _gui->render_tile(pm.coor);
                    _gui->render_piece(pm.coor);
                }

                _opt_moves.reset();
                _opt_selected.reset();
                return;
            }

            if (action.value() == DDDelta::E_UniqueAction::None)
            {
                std::cout << "regular action" << std::endl;
                _gui->render_tile(_opt_selected.value());

                _gui->render_tile(TO_BOARDCOOR(mouse_x, mouse_y));
                _gui->render_piece(TO_BOARDCOOR(mouse_x, mouse_y));

                for (auto pm : _opt_moves.lock()->captures)
                {
                    _gui->render_tile(pm.coor);
                    _gui->render_piece(pm.coor);
                }

                for (auto pm : _opt_moves.lock()->moves)
                {
                    _gui->render_tile(pm.coor);
                    _gui->render_piece(pm.coor);
                }

                _opt_moves.reset();
                _opt_selected.reset();
                return;
            }

            if ((action.value() != DDDelta::E_UniqueAction::None) && (action.value() != DDDelta::E_UniqueAction::Promote))
            {
                _gui->render_tile(_opt_selected.value());
                _gui->board_init();

                _opt_moves.reset();
                _opt_selected.reset();
                return;
            }

        }

    }
}


void Controller::run() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    _gui->board_init();
    SDL_Event event;

    while(!_quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case (SDL_QUIT):
                _quit = true;
                break;
            case (SDL_MOUSEMOTION):
                _handle_mouse_move();
                break;
            case (SDL_MOUSEBUTTONDOWN):
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    std::cout << "click" << std::endl;
                    if (_opt_selected)
                    {
                        std::cout << "selected" << std::endl;
                    }
                    _handle_mouse_click();
                    std::cout << "finish click" << std::endl;

//                    if (_handle_result(_chess_game->get_result()))
//                    {
//                        _in_game = false;
//                        // next game or quit
//                    }
                }
                break;
        }

    }

}

bool Controller::_handle_promote(SDL_Event promote, DDDelta::throwable::pawn_promote& e, DDDelta::BoardCoor coor) {
    switch (promote.type)
    {
        case (SDL_MOUSEBUTTONDOWN):
            if (promote.button.button == SDL_BUTTON_LEFT)
            {
                e.select_promotion(coor);
                _gui->board_init();
                return true;
            }

            break;
        default:
            assert(false);
    }
    return false;
}


bool Controller::_handle_result(DDDelta::E_Result res) {
    if (res == DDDelta::E_Result::UNFINISHED) return false;
//    _gui->render_result(res);
    return true;
}






NAMESPACE_BOBZHENG00_END