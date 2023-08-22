#include "GUI.h"
#include "code_utils.inc"


#define WHITE_TILE(x, y) (x % 2 != y % 2)

NAMESPACE_BOBZHENG00_START

void GUI::render_piece(DDDelta::BoardCoor coor) {

    if (_chess_game->get_piece(coor))
    {
        // std::cout<< coor.x << std::endl;
        // std::cout<< coor.y << std::endl;

        SDL_Texture* image_texture =
                SDL_CreateTextureFromSurface(renderer, _piece_map.at(_chess_game->get_piece(coor.x, coor.y
                        ).value()).get()); // TODO: texture -> map
        SDL_Rect image_rect = {(coor.x-1)*100, (8 - coor.y)*100, 100, 100};
        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);

        SDL_DestroyTexture(image_texture);
        // std::cout<< "success" << std::endl;
    }
}


void GUI::render_move(DDDelta::BoardCoor coor) {
    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _move_tile.get());
    SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
    SDL_DestroyTexture(image_texture);
}


void GUI::render_capture(DDDelta::BoardCoor coor) {
    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _capture_tile.get());
    SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
    SDL_DestroyTexture(image_texture);
}

void GUI::render_select(DDDelta::BoardCoor coor) {
    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _select_tile.get());
    SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
    SDL_DestroyTexture(image_texture);
}

void GUI::render_promote_selection(DDDelta::BoardCoor coor) {
    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _promote_shade.get());
    SDL_Rect image_rect = {0, 0, 800, 800};
    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
    SDL_DestroyTexture(image_texture);
    if (coor.y == 8)
    {
        for (i32 row = 0; row <= 3; row++)
        {
            render_tile({ coor.x, (coor.y-row) });
            switch (row){
                case 0:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::WhiteQueen).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y+row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 1:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::WhiteKnight).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y+row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 2:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::WhiteRook).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y+row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 3:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::WhiteBishop).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y+row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                default:
                    UNREACHABLE();
            }
        }
        return;
    }
    else if (coor.y == 1)
    {
        for (i32 row = 0; row <= 3; row++)
        {
            render_tile({ coor.x, (coor.y+row) }); //TODO: Add grey background circle
            switch (row){
                case 0:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::BlackQueen).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y-row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 1:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::BlackKnight).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y-row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 2:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::BlackRook).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y-row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                case 3:
                    image_texture =
                            SDL_CreateTextureFromSurface(renderer, _piece_map.at(DDDelta::constant::BlackBishop).get());
                    image_rect = {(coor.x-1)*100, (8-coor.y-row)*100, 100, 100};
                    SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                    SDL_DestroyTexture(image_texture);
                    break;
                default:
                    UNREACHABLE();
            }
        }
        return;
    }

}


void GUI::render_tile(DDDelta::BoardCoor coor){

//    if (_chess_game->get_selection())
//    {
//        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _select_tile.get());
//        SDL_Rect image_rect = {(coor.x-1)*100, (coor.y-1)*100, 100, 100};
//        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
//        std::cout << SDL_GetError() << std::endl;
//        return;
//    }

    if (_opt_hover)
    {
        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _hover_tile.get());
        SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
        _opt_hover.reset();
        SDL_DestroyTexture(image_texture);
        return;
    }


    if (WHITE_TILE(coor.x, coor.y))
    {
        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _white_tile.get());
        SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
        SDL_DestroyTexture(image_texture);
//        return;
    }
    else
    {
        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _black_tile.get());
        SDL_Rect image_rect = {(coor.x-1)*100, (8-coor.y)*100, 100, 100};
        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
        SDL_DestroyTexture(image_texture);
//        return;
    }

//    if (coor == something)
//    {
//        SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, _checked_tile.get());
//        SDL_Rect image_rect = {(coor.x-1)*100, (coor.y-1)*100, 100, 100};
//        SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
//        return;
//    }

}


void GUI::player_init() {
    SDL_Color text_color = { 255, 255, 255, 255};
    std::string player_black = _chess_game->player_black.name[0] + ' ' + _chess_game->player_black.name[1] + ' ' + _chess_game->player_black.name[2];
    SDL_Surface* surface = TTF_RenderText_Solid(TTF_OpenFont("../font.ttf", 24), player_black.c_str(), text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { 850, 50, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    std::string player_white = _chess_game->player_white.name[0] + ' ' + _chess_game->player_white.name[1] + ' ' + _chess_game->player_white.name[2];
    surface = TTF_RenderText_Solid(TTF_OpenFont("../font.ttf", 24), player_white.c_str(), text_color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textRect = { 850, 725, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void GUI::board_init() {

    player_init();
    for (i32 row = 1; row <= 8; row++) {
        for (i32 col = 1; col <= 8; col++) {
            render_tile({ col, row });
            render_piece({ col, row });

        }
    }

    SDL_RenderPresent(renderer);

}


void GUI::set_hover(DDDelta::BoardCoor hover_coor) {
    _opt_hover = hover_coor;
}


//void GUI::render_result(DDDelta::E_Result res) {
//    SDL_Surface *surface = TTF_RenderText_Solid(_font24.get(), _chess_game->player_white.name[0].c_str(), {255, 255, 255, 255});
//    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//    SDL_Rect textRect = {850, 387, surface->w, surface->h};
//    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
//    // TODO: highlight winner and display result
//}

NAMESPACE_BOBZHENG00_END

