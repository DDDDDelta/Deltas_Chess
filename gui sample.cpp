#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include "SDL2/include/SDL.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#include <SDL_image.h>
#include "GUI.h"
#include <map>



int main(int argc, char *argv[])
{
    std::string board[8][8] = {{"BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"},
                               {"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"},
                               {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                               {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                               {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                               {"empty", "empty", "empty", "empty", "empty", "empty", "empty", "empty"},
                               {"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"},
                               {"WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"}};

    std::map<std::string, SDL_Surface*> image_map;

    image_map["WP"] = IMG_Load("../imgs/wP.png");
    image_map["WN"] = IMG_Load("../imgs/wN.png");
    image_map["WB"] = IMG_Load("../imgs/wB.png");
    image_map["WR"] = IMG_Load("../imgs/wR.png");
    image_map["WQ"] = IMG_Load("../imgs/wQ.png");
    image_map["WK"] = IMG_Load("../imgs/wK.png");
    image_map["BP"] = IMG_Load("../imgs/bP.png");
    image_map["BN"] = IMG_Load("../imgs/bN.png");
    image_map["BB"] = IMG_Load("../imgs/bB.png");
    image_map["BR"] = IMG_Load("../imgs/bR.png");
    image_map["BQ"] = IMG_Load("../imgs/bQ.png");
    image_map["BK"] = IMG_Load("../imgs/bK.png");

//    std::cout << SDL_GetError();
    std::string A = "Bob Zheng";
    SDL_Init(SDL_INIT_VIDEO);
    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 800, 0);

    // Create an SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the color for drawing text
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Set the font for text rendering (assuming you have a TTF font file)
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("../font.ttf", 24);
    std::cout << SDL_GetError() << std::endl;
    // Set the rendering color for text
    SDL_Color textColor = {255, 255, 255, 255};

//    SDL_Rect square = {(0, 0, 1100, 800)};
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderFillRect(renderer, &square);
//    SDL_DestroyTexture(texture);
//    SDL_FreeSurface(surface);


    // Render player names on the left side of the window
    SDL_Surface* surface = TTF_RenderText_Solid(font, A.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect text_rect = {850, 50, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &text_rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);


    surface = TTF_RenderText_Solid(font, "Steven Shi", textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_rect = {850, 725, surface->w, surface->h};
//    textRect.y += surface->h + 10;
    SDL_RenderCopy(renderer, texture, nullptr, &text_rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);


    bool isWhite = true;
    for (std::uint8_t j = 1; j <= 8; j++) {
        for (std::uint8_t i = 1; i <= 8; i++) {
            if (isWhite){
                SDL_Rect square = {(i-1)*100, (j-1)*100, 100, 100};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &square);
                isWhite = !isWhite;
            }
            else{
                SDL_Rect square = {(i - 1) * 100, (j - 1) * 100, 100, 100};
                SDL_SetRenderDrawColor(renderer,101, 147, 52, 255);
                SDL_RenderFillRect(renderer, &square);
                isWhite = !isWhite;
            }
            if (board[j-1][i-1] != "empty"){
                SDL_Texture* img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(board[j-1][i-1]));

                SDL_Rect image_rect = {(i-1)*100, (j-1)*100, 100, 100};
                SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);

            }
        }
        isWhite=!isWhite;
    }


    // Present the renderer to display the window


    // Main loop
    bool quit = false;
    bool selecting = false;
    SDL_Event event;
    int x, y;
    int coorx = 0;
    int coory = 0;
    int poss[2] {4, 4};
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            SDL_GetMouseState(&x, &y);
            if (x < 800 && y < 800)
            {
                if ((event.type == SDL_MOUSEMOTION) && !selecting)
                {

                        /* Redraw the previous tile */

                        if (coorx != 0 && coory != 0 && ((x/100+1) != coorx | (y/100+1) != coory))
                        {
                            std::cout << "leaving previous tile" << std::endl;
                            if (coorx % 2 == coory % 2){
                                SDL_Rect square = {(coorx-1)*100, (coory-1)*100, 100, 100};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &square);

                            }
                            else
                            {
                                SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                                SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                                SDL_RenderFillRect(renderer, &square);
                            }

                            if (board[coory-1][coorx-1] != "empty")
                            {
                                SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(
                                        board[coory - 1][coorx - 1]));

                                SDL_Rect image_rect = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                                SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);
                            }

                        }

                        /* Highlight the current tile under the cursor */

                        coorx = x / 100 + 1;
                        coory = y / 100 + 1;
//                        std::cout << "x: " << coorx << "  y: " << coory << std::endl;
                        SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                        SDL_SetRenderDrawColor(renderer,200, 147, 52, 255);
                        SDL_RenderFillRect(renderer, &square);

                        if (board[coory-1][coorx-1] != "empty")
                        {
                            SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(
                                    board[coory - 1][coorx - 1]));

                            SDL_Rect image_rect = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                            SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);
                        }




                }
            }
            /* User clicking action */

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    if ((board[coory - 1][coorx - 1] != "empty") && ((x/100 + 1) == coorx) && ((y/100 + 1) == coory))
                    {
                        if (selecting) // Undo select (clear all highlights)
                        {
                            if (coorx % 2 == coory % 2)
                            {
                                SDL_Rect square = {(coorx-1)*100, (coory-1)*100, 100, 100};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &square);

                            }
                            else
                            {
                                SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                                SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                                SDL_RenderFillRect(renderer, &square);
                            }

                            if (poss[0] % 2 == poss[1] % 2) // require a for loop when deal with real case
                            {
                                SDL_Rect square = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer, &square);

                            }
                            else
                            {
                                SDL_Rect square = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                                SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                                SDL_RenderFillRect(renderer, &square);
                            }

                            selecting = false;

                        }
                        else // Select piece that can be moved
                        {
                            selecting = true;

                            SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer,100, 147, 252, 255);
                            SDL_RenderFillRect(renderer, &square);

                            SDL_Rect poss_rect = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer,250, 147, 152, 255);
                            SDL_RenderFillRect(renderer, &poss_rect);

                        }

                        // restore pieces that is highlighted (capture)
                        if (board[poss[1]-1][poss[0]-1] != "empty")
                        {
                            SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(
                                    board[poss[0] - 1][poss[1] - 1]));
                            SDL_Rect image_rect = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                            SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);
                        }

                        SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(
                                board[coory - 1][coorx - 1]));
                        SDL_Rect image_rect = {(coorx - 1) * 100, (coory - 1) * 100 - 8, 100, 100};
                        SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);


                    }
                    else if (((x/100 + 1) == poss[0]) && ((y/100 + 1) == poss[1]) && selecting) // execute_move == true
                    {
                        if (coorx % 2 == coory % 2) // remove piece in its original place
                        {
                            SDL_Rect square = {(coorx-1)*100, (coory-1)*100, 100, 100};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &square);

                        }
                        else
                        {
                            SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                            SDL_RenderFillRect(renderer, &square);
                        }

                        if (poss[0] % 2 == poss[1] % 2) // clear possible move highlight (require a for loop when deal with real case)
                        {
                            SDL_Rect square = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &square);

                        }
                        else
                        {
                            SDL_Rect square = {(poss[0]-1) * 100, (poss[1]-1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                            SDL_RenderFillRect(renderer, &square);
                        }

                        /* manipulate board */
                        board[y/100][x/100] = std::move(board[coory - 1][coorx - 1]);
                        board[coory - 1][coorx - 1] = "empty";


                        coorx = x / 100 + 1;
                        coory = y / 100 + 1;


                        if (coorx % 2 == coory % 2)
                        {
                            SDL_Rect square = {(coorx-1)*100, (coory-1)*100, 100, 100};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &square);

                        }
                        else
                        {
                            SDL_Rect square = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                            SDL_SetRenderDrawColor(renderer,101, 147, 52, 100);
                            SDL_RenderFillRect(renderer, &square);
                        }
                        std::cout << "x: " << coorx << "  y: " << coory << std::endl;
                        SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image_map.at(
                                board[coory - 1][coorx - 1]));
                        SDL_Rect image_rect = {(coorx - 1) * 100, (coory - 1) * 100, 100, 100};
                        SDL_RenderCopy(renderer, img_texture, nullptr, &image_rect);


                        std::cout << "move" << std::endl;
                        selecting = false;

                        /* change player */
                    }
                }
                std::cout << "click" << std::endl;
            }

            SDL_RenderPresent(renderer);
        }




//        A = "1111111";
//        // Render player names on the left side of the window
//        SDL_Surface* surface = TTF_RenderText_Solid(font, A.c_str(), textColor);
//        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//        SDL_Rect textRect = {20, 20, surface->w, surface->h};
//        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
//        SDL_DestroyTexture(texture);
//        SDL_FreeSurface(surface);
//
//        surface = TTF_RenderText_Solid(font, "Steven Shi", textColor);
//        texture = SDL_CreateTextureFromSurface(renderer, surface);
//        textRect.y += surface->h + 10;
//        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
//        SDL_DestroyTexture(texture);
//        SDL_FreeSurface(surface);
//
//
//        // Present the renderer to display the window
//        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    TTF_CloseFont(font);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


