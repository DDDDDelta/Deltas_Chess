#include <iostream>
#include "SDL2_ttf/include/SDL_ttf.h"
#include <SDL.h>

#include "GUI.h"
#include "code_utils.inc"


#define WHITE_TILE(x, y) (x % 2 != y % 2)

NAMESPACE_BOBZHENG00_START
GUI::GUI(const DDDelta::ChessGame* chess_game) :
        _texture_map(), _chess_game(chess_game), window(), renderer(), _white_texture(), _black_texture(), _hover_texture(),
        _select_texture(), _promote_texture(), _capture_texture(), _opt_hover(), _checked_texture(), _move_texture(), _font24()
{
    // initialize SDL features
    TTF_Init();
    window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 800, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // load pieces' textures in to a map using Piece Type as keys
    _texture_map[DDDelta::constant::WhitePawn] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wP.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::WhiteKnight] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wN.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::WhiteBishop] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wB.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::WhiteRook] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wR.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::WhiteQueen] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wQ.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::WhiteKing] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/wK.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackPawn] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bP.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackKnight] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bN.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackBishop] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bB.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackRook] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bR.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackQueen] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bQ.png")), SDL_DestroyTexture };
    _texture_map[DDDelta::constant::BlackKing] = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/bK.png")), SDL_DestroyTexture };

    // load board related textures and fonts as unique pointers
    _white_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/whiteT.png")), SDL_DestroyTexture };
    _black_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/blackT.png")), SDL_DestroyTexture };
    _hover_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/hoverT.png")), SDL_DestroyTexture };
    _select_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/selectT.png")), SDL_DestroyTexture };
    _move_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/moveT.png")), SDL_DestroyTexture };
    _capture_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/captureT.png")), SDL_DestroyTexture };
    _promote_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/promoteB.png")), SDL_DestroyTexture };
    _checked_texture = { SDL_CreateTextureFromSurface(renderer, IMG_Load("../imgs/checkedT.png")), SDL_DestroyTexture };
    _font24 = {TTF_OpenFont("../font.ttf", 24), TTF_CloseFont};

    // reset private optional variables
    _opt_hover.reset();
}


/* all the coor parameters are meant to taking in algebraic notations of Chess with bottom-left corner named as (1, 1) */


void GUI::render_piece(DDDelta::BoardCoor coor) {
    // if there is a valid piece is on the coor
    if (_chess_game->get_piece(coor)) {
        SDL_Rect image_rect = {(coor.x-1)*100, (8 - coor.y)*100, 100, 100};
        SDL_RenderCopy(renderer, _texture_map.at(_chess_game->get_piece(coor.x, coor.y).value()).get(), nullptr, &image_rect);
    }
}


void GUI::render_tile(DDDelta::BoardCoor coor) {
    // if a tile is hovered by player
    if (_opt_hover) {
        player_init();
        SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
        SDL_RenderCopy(renderer, _hover_texture.get(), nullptr,&image_rect);
        _opt_hover.reset();
        return;
    }

    // draw a regular white or black tile
    if (WHITE_TILE(coor.x, coor.y)) {
        SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
        SDL_RenderCopy(renderer, _white_texture.get(), nullptr, &image_rect);
    }
    else {
        SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
        SDL_RenderCopy(renderer, _black_texture.get(), nullptr, &image_rect);
    }

    // draw a checked warning upon the tile
    //if (coor == something)
    //{
    //    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _checked_tile.get());
    //    SDL_Rect image_rect = {(coor.x-1)*100, (coor.y-1)*100, 100, 100};
    //    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
    //    return;
    //}
}


void GUI::render_move(DDDelta::BoardCoor coor){
    SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
    SDL_RenderCopy(renderer, _move_texture.get(), nullptr,&image_rect);
}


void GUI::render_capture(DDDelta::BoardCoor coor){
    SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
    SDL_RenderCopy(renderer, _capture_texture.get(), nullptr,&image_rect);
}


void GUI::render_select(DDDelta::BoardCoor coor){
    SDL_Rect image_rect = { (coor.x - 1) * 100, (8 - coor.y) * 100, 100, 100 };
    SDL_RenderCopy(renderer, _select_texture.get(), nullptr, &image_rect);
}


void GUI::render_possible_moves(const std::shared_ptr<const DDDelta::PossibleMovement>& movement){
    // if a piece is selected, draw all the possible movements
    if (_chess_game->get_selection()) {
        for (auto pm : movement->captures) {
            render_capture(pm.coor);
            render_piece(pm.coor);
        }

        for (auto pm : movement->moves) {
            render_move(pm.coor);
            render_piece(pm.coor);
        }
    }
    // if no piece is selected, erase all the possible movements
    else {
        for (auto pm : movement->captures) {
            render_tile(pm.coor);
            render_piece(pm.coor);
        }
        for (auto pm : movement->moves) {
            render_tile(pm.coor);
            render_piece(pm.coor);
        }
    }
}


void GUI::render_promote_selection(DDDelta::BoardCoor coor) {
    // render the black shade to emphasize the promote selections
    SDL_Rect image_rect = { 0, 0, 800, 800 };
    SDL_RenderCopy(renderer, _promote_texture.get(), nullptr,&image_rect);

    // based on whether the player has clicked the top rank or the bottom rank, draw the promote selection of correct side
    render_tile({ coor.x, y_position<1>[coor.y] }); //TODO: Add grey background circle
    image_rect = {(coor.x - 1)*100, (8 - y_position<1>[coor.y])*100, 100, 100};
    SDL_RenderCopy(renderer, _texture_map.at({ piece_color<1>[coor.y], DDDelta::E_PieceType::Queen }).get(), nullptr, &image_rect);

    render_tile({ coor.x, y_position<2>[coor.y] });
    image_rect = { (coor.x - 1)*100, (8 - y_position<2>[coor.y])*100, 100, 100 };
    SDL_RenderCopy(renderer, _texture_map.at({ piece_color<2>[coor.y], DDDelta::E_PieceType::Knight }).get(), nullptr, &image_rect);

    render_tile({ coor.x, y_position<3>[coor.y] });
    image_rect = { (coor.x - 1)*100, (8 - y_position<3>[coor.y])*100, 100, 100 };
    SDL_RenderCopy(renderer, _texture_map.at({ piece_color<3>[coor.y], DDDelta::E_PieceType::Rook }).get(), nullptr, &image_rect);

    render_tile({ coor.x, y_position<4>[coor.y] });
    image_rect = {(coor.x - 1)*100, (8 - y_position<4>[coor.y])*100, 100, 100};
    SDL_RenderCopy(renderer, _texture_map.at({ piece_color<4>[coor.y], DDDelta::E_PieceType::Bishop }).get(), nullptr, &image_rect);
}


void GUI::player_init() {
    SDL_Color text_color = {255, 255, 255, 255};
    std::string player_black = _chess_game->player_black.name[0] + ' ' + _chess_game->player_black.name[1] + ' ' + _chess_game->player_black.name[2];
    SDL_Surface* surface = TTF_RenderText_Solid(_font24.get(), player_black.c_str(), text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {850, 50, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    std::string player_white = _chess_game->player_white.name[0] + ' ' + _chess_game->player_white.name[1] + ' ' + _chess_game->player_white.name[2];
    surface = TTF_RenderText_Solid(_font24.get(), player_white.c_str(), text_color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textRect = {850, 725, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void GUI::board_init() {

    player_init();
    for (i32 row = 1; row <= 8; row++) {
        for (i32 col = 1; col <= 8; col++) {
            render_tile(DDDelta::BoardCoor {col, row});
            render_piece(DDDelta::BoardCoor {col, row});
        }
    }

    SDL_RenderPresent(renderer);

}

// provide a setter for controller to record the hovered tile
void GUI::set_hover(DDDelta::BoardCoor hover_coor) {
    _opt_hover = hover_coor;
}

//void GUI::render_result(DDDelta::E_Result res){
//    SDL_Surface *surface = TTF_RenderText_Solid(_font24.get(), _chess_game->player_white.name[0].c_str(), {255, 255, 255, 255});
//    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//    SDL_Rect textRect = {850, 387, surface->w, surface->h};
//    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
//    // TODO: highlight winner and display result
//}

NAMESPACE_BOBZHENG00_END

