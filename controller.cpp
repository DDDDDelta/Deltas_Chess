//
// Created by bobzheng on 2023/7/20.
//

#include <iostream>

#include "GUI.h"
#include "moves.h"
#include "controller.h"
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


bool Controller::_handle_board_operation() {

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // pass the check only when the game is alive and player clicks on the board
    if (!_in_game || !ON_BOARD(mouse_x, mouse_y)) return false;

    // if there is no previous selection, select piece based on player input
    if (!_opt_selected) {
        _wp_moves = _chess_game->select_piece(TO_CHESSBOARDCOOR(mouse_x, mouse_y));

        // if the selected piece can't be moved, reselect
        if (_wp_moves.lock() == nullptr) return false;

        // remember the coor of selected piece and render it with all possible movements
        _opt_selected = TO_CHESSBOARDCOOR(mouse_x, mouse_y);
        _gui->render_select(_opt_selected.value());
        _gui->render_piece(_opt_selected.value());
        _gui->render_possible_moves(_wp_moves.lock(), _opt_selected);
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
            _opt_selected.reset();
            _gui->render_possible_moves(_wp_moves.lock(), _opt_selected);
            _wp_moves.reset();
            return false;
        }
        // regular action
        else if (action.value() == DDDelta::E_UniqueAction::None || action.value() == DDDelta::E_UniqueAction::DoublePawnPush) {
            // erase piece's original location & possible movements highlights and render new location of the piece
            _gui->render_tile(_opt_selected.value());
            _opt_selected.reset();
            _gui->render_possible_moves(_wp_moves.lock(), _opt_selected);
            _gui->render_tile(TO_BOARDCOOR(mouse_x, mouse_y));
            _gui->render_piece(TO_BOARDCOOR(mouse_x, mouse_y));
            _wp_moves.reset();

            return true;
        }
        // other unique action
        else {
            // rerender the whole board
            _gui->board_init();
            _wp_moves.reset();
            _opt_selected.reset();
            return true;
        }
    }

    return false;
}


void Controller::run() {
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
                    // if a movement is successfully executed and game is finished
                    if (_handle_board_operation() && (_chess_game->get_result() != DDDelta::E_Result::UNFINISHED)) {
                        std::cout << "game is finished" << std::endl;
                        // display the result and let player choose if they want to ...
                        E_AfterGameDecision decision =  _handle_result();
                        // 1. quit the game
                        if (decision == E_AfterGameDecision::Quit) {
                            goto Quit;
                        // 2. switch side and restart the game with the same two players
                        } else if (decision == E_AfterGameDecision::Switch) {
                            _gui->board_init();
                            // TODO: Reset chess_game and switch side
                        // 3. restart the whole game from beginning with two new players
                        } else if (decision == E_AfterGameDecision::Restart) {
                            // TODO: return something, so main knows to restart the game from beginning to sign up new players
                            return;
                        }
                    }

                }
                break;
//            case (timer):
//                std::cout << "update timer" << std::endl;
//                break;
        }

    }
Quit:
    SDL_DestroyRenderer(_gui->renderer);
    SDL_DestroyWindow(_gui->window);
    IMG_Quit();
    SDL_Quit();
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


E_AfterGameDecision Controller::_handle_result() {
    _gui->render_result(_chess_game->get_result());
//    _gui->render_result(DDDelta::E_Result::BLACK_WIN);

    SDL_Event result;
    while (true) {
        SDL_RenderPresent(_gui->renderer);
        SDL_WaitEvent(&result);
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        switch (result.type) {
            case(SDL_QUIT):
                return E_AfterGameDecision::Quit;
            case(SDL_MOUSEMOTION):
                if ((mouse_x >= 850) && (mouse_x <=1000) && (mouse_y >= 300) && (mouse_y <= 325)) {
                    _gui->render_finish_selection({250, 147, 152, 255}, {255, 255, 255, 255}, {255, 255, 255, 255});
                } else if ((mouse_x >= 850) && (mouse_x <=1050) && (mouse_y >= 350) && (mouse_y <= 375)) {
                    _gui->render_finish_selection({255, 255, 255, 255}, {250, 147, 152, 255}, {255, 255, 255, 255});
                } else if ((mouse_x >= 850) && (mouse_x <=950) && (mouse_y >= 400) && (mouse_y <= 425)) {
                    _gui->render_finish_selection({255, 255, 255, 255}, {255, 255, 255, 255}, {250, 147, 152, 255});
                } else {
                    _gui->render_finish_selection({255, 255, 255, 255}, {255, 255, 255, 255}, {255, 255, 255, 255});
                }
                break;
            case(SDL_MOUSEBUTTONDOWN):
                if (result.button.button == SDL_BUTTON_LEFT) {
                    if ((mouse_x >= 850) && (mouse_x <=1000) && (mouse_y >= 300) && (mouse_y <= 325)) {
                        _gui->render_finish_selection({250, 147, 152, 255}, {255, 255, 255, 255}, {255, 255, 255, 255});
                        std::cout << "clicking switch side" << "\n";
                        return E_AfterGameDecision::Switch;
                    } else if ((mouse_x >= 850) && (mouse_x <=1050) && (mouse_y >= 350) && (mouse_y <= 375)) {
                        _gui->render_finish_selection({255, 255, 255, 255}, {250, 147, 152, 255}, {255, 255, 255, 255});
                        std::cout << "clicking restart game" << "\n";
                        return E_AfterGameDecision::Restart;
                    } else if ((mouse_x >= 850) && (mouse_x <=950) && (mouse_y >= 400) && (mouse_y <= 425)) {
                        _gui->render_finish_selection({255, 255, 255, 255}, {255, 255, 255, 255}, {250, 147, 152, 255});
                        std::cout << "Quit" << "\n";
                        return E_AfterGameDecision::Quit;
                    }
                }
                break;
        }
    }
    // TODO: wait for player's choice to quit or restart the game
}


NAMESPACE_BOBZHENG00_END