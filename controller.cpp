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
    if (!ON_BOARD(mouse_x, mouse_y)) {
        if (opt_hover) {
            _gui->render_tile(opt_hover.value());
            _gui->render_piece(opt_hover.value());
            opt_hover.reset();
        }
        return;
    }

    // redraw previous hovered tile and piece
    if (NEED_REDRAW_PREVIOUS_TILE(mouse_x, mouse_y, opt_hover->x, opt_hover->y) && opt_hover) {
        _gui->render_tile(opt_hover.value());
        _gui->render_piece(opt_hover.value());
    }

    // Update current hover tile
    opt_hover = TO_CHESSBOARDCOOR(mouse_x, mouse_y);

    // Draw hovered tile and piece
    _gui->set_hover(opt_hover.value());
    _gui->render_tile(opt_hover.value());
    _gui->render_piece(opt_hover.value());

}


void Controller::_handle_mouse_click() {

    if (!_in_game) return;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (ON_BOARD(mouse_x, mouse_y)) {
        // if there is no previous selection, select piece based on player input
        if (!_chess_game->get_selection()) {
            _wp_moves = _chess_game->select_piece(TO_CHESSBOARDCOOR(mouse_x, mouse_y));

            // if the selected piece can't be moved, reselect
            if (_wp_moves.lock() == nullptr) return;

            // remember the coor of selected piece and render it with all possible movements
            _opt_selected = _chess_game->get_selection();
            _gui->render_select(_chess_game->get_selection().value());
            _gui->render_piece(_chess_game->get_selection().value());
            _gui->render_possible_moves(_wp_moves.lock());

            return;
        }
        // if there is a piece selected
        else {
            std::optional<DDDelta::E_UniqueAction> action;

            // try to execute player's decision of piece move
            try {
                action = _chess_game->execute_move(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
            }
            // catch pawn promote exception
            catch(DDDelta::throwable::pawn_promote& e) {
                // render the pawn promote selection interface
                _gui->board_init();
                _gui->render_promote_selection(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
                SDL_RenderPresent(_gui->renderer);

                // switch into promote event loop
                SDL_Event promote;
                while (true) {
                    SDL_WaitEvent(&promote);
                    // break out the promote event loop if pawn promote has been successfully executed
                    if (_handle_promote(promote, e)) {
                        action = DDDelta::E_UniqueAction::Promote;
                        break;
                    }
                }
            }

            // render board and pieces after movements have been executed

            // illegal action
            if (!action) {
                // rerender the selected piece and erase possible movements highlights
                _gui->render_tile(_opt_selected.value());
                _gui->render_piece(_opt_selected.value());
                _gui->render_possible_moves(_wp_moves.lock());
                _wp_moves.reset();
                _opt_selected.reset();
            }
            // regular action
            else if (action.value() == DDDelta::E_UniqueAction::None || action.value() == DDDelta::E_UniqueAction::DoublePawnPush) {
                // erase piece's original location & possible movements highlights and render new location of the piece
                _gui->render_tile(_opt_selected.value());
                _gui->render_tile(TO_BOARDCOOR(mouse_x, mouse_y));
                _gui->render_piece(TO_BOARDCOOR(mouse_x, mouse_y));
                _gui->render_possible_moves(_wp_moves.lock());
                _wp_moves.reset();
                _opt_selected.reset();
            }
            // other unique action
            else {
                // rerender the whole board
                _gui->board_init();
                _wp_moves.reset();
                _opt_selected.reset();
            }
        }
    }
}


void Controller::run() {
    SDL_Init(SDL_INIT_VIDEO);
    _gui->board_init();

    SDL_Event event;
    while (true)
    {
        SDL_WaitEvent(&event);
        SDL_RenderPresent(_gui->renderer);
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
                if (event.button.button == SDL_BUTTON_LEFT) {
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
            if (promote.button.button == SDL_BUTTON_LEFT) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                // return if promotion has been successfully executed or not
                return e.select_promotion(TO_CHESSBOARDCOOR(mouse_x, mouse_y));
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