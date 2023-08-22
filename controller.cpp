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
(board_x != 0 && board_y != 0 && ((mouse_x / 100 + 1) != board_x || (8 - mouse_y / 100) != board_y))
#define TO_BOARDCOOR(x, y) (DDDelta::BoardCoor(x / 100 + 1, y / 100 + 1))
#define TO_CHESSBOARDCOOR(x, y) (DDDelta::BoardCoor(x / 100 + 1, 8 - y / 100))
NAMESPACE_BOBZHENG00_START


void Controller::_handle_mouse_move() {
    if (!_in_game || _opt_selected) return;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // if it goes out of board then reset hover and maybe redraw
    if (!ON_BOARD(mouse_x, mouse_y))
    {
        if (opt_hover)
        {
            _gui->render_tile(opt_hover.value());
            _gui->render_piece(opt_hover.value());
            opt_hover.reset();
        }
        return;
    }

    // redraw previous hovered tile and piece
    if (NEED_REDRAW_PREVIOUS_TILE(mouse_x, mouse_y, opt_hover->x, opt_hover->y) && opt_hover)
    {
        std::cout << "leaving" << std::endl;
        _gui->render_tile(opt_hover.value());
        _gui->render_piece(opt_hover.value());
    }

    // Update current tile
    opt_hover = TO_CHESSBOARDCOOR(mouse_x, mouse_y);

    // Draw hovered tile and piece
    _gui->set_hover(opt_hover.value());
    _gui->render_tile(opt_hover.value());
    _gui->render_piece(opt_hover.value());
    SDL_RenderPresent(_gui->renderer);
}


void Controller::_handle_mouse_click() {

    if (!_in_game) return;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (ON_BOARD(mouse_x, mouse_y))
    {
        // if there is no previous selection
        if (!_chess_game->get_selection())
        {
            _wp_moves = _chess_game->select_piece(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
            _opt_selected = _chess_game->get_selection();

            if (_wp_moves.lock() == nullptr) return;

            _gui->render_select(_chess_game->get_selection().value());
            _gui->render_piece(_chess_game->get_selection().value());
            _gui->render_possible_moves(_wp_moves.lock());

            SDL_RenderPresent(_gui->renderer);
            return;
        }
        else
        {
            std::optional<DDDelta::E_UniqueAction> action;
            try
            {
                action = _chess_game->execute_move(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
            }
            catch(DDDelta::throwable::pawn_promote& e)
            {
                _gui->board_init();
                _gui->render_promote_selection(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
                SDL_RenderPresent(_gui->renderer);

                SDL_Event promote;
                while (true)
                {
                    SDL_WaitEvent(&promote);
                    if (_handle_promote(promote, e)) break;
                }
            }

            if (!action) // illegal action + promote
            {
                _gui->render_tile(_opt_selected.value());
                _gui->render_piece(_opt_selected.value());
                _gui->render_possible_moves(_wp_moves.lock());
                _wp_moves.reset();
                _opt_selected.reset();
                SDL_RenderPresent(_gui->renderer);
            }
            else if (action.value() == DDDelta::E_UniqueAction::None || action.value() == DDDelta::E_UniqueAction::DoublePawnPush) // regular action
            {
                _gui->render_tile(_opt_selected.value());
                _gui->render_tile(TO_BOARDCOOR(mouse_x, mouse_y));
                _gui->render_piece(TO_BOARDCOOR(mouse_x, mouse_y));
                _gui->render_possible_moves(_wp_moves.lock());
                _wp_moves.reset();
                _opt_selected.reset();
                SDL_RenderPresent(_gui->renderer);
            }
            else // other unique action that is not promote
            {
                _gui->render_tile(_opt_selected.value());
                _gui->board_init();
                _wp_moves.reset();
                _opt_selected.reset();
            }

        }

    }

}


void Controller::run() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    _gui->board_init();
    SDL_Event event;

    while (true)
    {
        SDL_WaitEvent(&event);
//        event.type = timer;
//        SDL_PushEvent(&event);

        switch (event.type)
        {
            case (SDL_QUIT):
                goto Quit;
            case (SDL_MOUSEMOTION):

                _handle_mouse_move();
                break;
            case (SDL_MOUSEBUTTONDOWN):
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    _handle_mouse_click();

//                    if (_handle_result(_chess_game->get_result()))
//                    {
//                        _in_game = false;
//                        // next game or quit
//                    }
                }
                break;
//            case (timer):
//                std::cout << "update timer" << std::endl;
//                break;
        }

    }
Quit:
    return;
}

bool Controller::_handle_promote(SDL_Event promote, DDDelta::throwable::pawn_promote& e) {
    switch (promote.type)
    {
        case (SDL_MOUSEBUTTONDOWN):
            if (promote.button.button == SDL_BUTTON_LEFT)
            {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                e.select_promotion(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
                _gui->board_init();
//                _wp_moves.reset();
//                _opt_selected.reset();
                return true;
            }

            break;
        default:
            UNREACHABLE();
    }
    return false;
}


bool Controller::_handle_result(DDDelta::E_Result res) {
    if (res == DDDelta::E_Result::UNFINISHED) return false;
//    _gui->render_result(res);
    return true;
}


NAMESPACE_BOBZHENG00_END