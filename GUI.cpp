//
// Created by bobzheng on 2023/7/9.
//

#include <iostream>
#include "code_utils.inc"
#include "GUI.h"
#include "chess_game.h"
#include "pieces.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <map>
#include <filesystem>

NAMESPACE_BOBZHENG00_START

void GUI::player_init(DDDelta::ChessGame chess_game, SDL_Renderer *renderer) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("../font.ttf", 24);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, chess_game.player_white.name.last_name.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {850, 50, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    surface = TTF_RenderText_Solid(font, chess_game.player_black.name.last_name.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textRect = {850, 725, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void GUI::board_init(DDDelta::ChessGame chess_game) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    player_init(chess_game, renderer);

    std::map<DDDelta::Piece, SDL_Surface*> image_map;
    image_map[DDDelta::WhitePawn] = IMG_Load("../imgs/wP.png");
    image_map[DDDelta::WhiteKnight] = IMG_Load("../imgs/wN.png");
    image_map[DDDelta::WhiteBishop] = IMG_Load("../imgs/wB.png");
    image_map[DDDelta::WhiteRook] = IMG_Load("../imgs/wR.png");
    image_map[DDDelta::WhiteQueen] = IMG_Load("../imgs/wQ.png");
    image_map[DDDelta::WhiteKing] = IMG_Load("../imgs/wK.png");
    image_map[DDDelta::BlackKing] = IMG_Load("../imgs/bP.png");
    image_map[DDDelta::BlackKnight] = IMG_Load("../imgs/bN.png");
    image_map[DDDelta::BlackBishop] = IMG_Load("..imgs/bB.png");
    image_map[DDDelta::BlackRook] = IMG_Load("../imgs/bR.png");
    image_map[DDDelta::BlackQueen] = IMG_Load("../imgs/bQ.png");
    image_map[DDDelta::BlackKing] = IMG_Load("../imgs/bK.png");

    bool is_white = true;
    for (std::uint8_t j = 1; j <= 8; j++) {
        for (std::uint8_t i = 1; i <= 8; i++) {

            if (is_white) {
                SDL_Rect square = {(i - 1) * 100, (j - 1) * 100, 100, 100};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &square);
                is_white = !is_white;
            }
            else {
                SDL_Rect square = {(i - 1) * 100, (j - 1) * 100, 100, 100};
                SDL_SetRenderDrawColor(renderer,101, 147, 52, 255);
                SDL_RenderFillRect(renderer, &square);
                is_white = !is_white;
            }

            if (chess_game.get_piece((j-1), (i-1)) != DDDelta::Empty){
                SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(chess_game.get_piece((j-1), (i-1))));
                SDL_Rect image_rect = {(i-1)*100, (j-1)*100, 100, 100};
                SDL_RenderCopy(renderer, image_texture, nullptr, &image_rect);
                SDL_RenderPresent(renderer);
            }

        }

        is_white = !is_white;

    }

}

NAMESPACE_BOBZHENG00_END

