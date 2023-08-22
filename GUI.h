#ifndef DELTAS_CHESS_GUI_H
#define DELTAS_CHESS_GUI_H

#include <map>
#include <utility>
#include <memory>
#include <functional>
#include <cassert>
#include <ranges>

#include "SDL_image.h"
#include "chess_game.h"
#include "SDL2_ttf/include/SDL_ttf.h"
#include "SDL2/include/SDL.h"
#include "code_utils.inc"

USING_DDDELTA_INTDEF

NAMESPACE_BOBZHENG00_START


class GUI {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;

        explicit GUI(const DDDelta::ChessGame* chess_game) :
        _piece_map(), _chess_game(chess_game), window(), renderer(), _white_tile(), _black_tile(), _hover_tile(),
        _select_tile(), _promote_shade(), _capture_tile(), _opt_hover(), _checked_tile(), _move_tile(), _font24()
        {
            _piece_map[DDDelta::constant::WhitePawn] = { IMG_Load("../imgs/wP.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::WhiteKnight] = { IMG_Load("../imgs/wN.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::WhiteBishop] = { IMG_Load("../imgs/wB.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::WhiteRook] = { IMG_Load("../imgs/wR.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::WhiteQueen] = { IMG_Load("../imgs/wQ.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::WhiteKing] = { IMG_Load("../imgs/wK.png"), SDL_FreeSurface };

            _piece_map[DDDelta::constant::BlackPawn] = { IMG_Load("../imgs/bP.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::BlackKnight] = { IMG_Load("../imgs/bN.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::BlackBishop] = { IMG_Load("../imgs/bB.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::BlackRook] = { IMG_Load("../imgs/bR.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::BlackQueen] = { IMG_Load("../imgs/bQ.png"), SDL_FreeSurface };
            _piece_map[DDDelta::constant::BlackKing] = { IMG_Load("../imgs/bK.png"), SDL_FreeSurface };

            window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 800, 0);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            _white_tile = { IMG_Load("../imgs/whiteT.png"), SDL_FreeSurface };
            _black_tile = { IMG_Load("../imgs/blackT.png"), SDL_FreeSurface };
            _hover_tile = { IMG_Load("../imgs/hoverT.png"), SDL_FreeSurface };
            _select_tile = { IMG_Load("../imgs/selectT.png"), SDL_FreeSurface };
            _move_tile = { IMG_Load("../imgs/moveT.png"), SDL_FreeSurface };
            _capture_tile = { IMG_Load("../imgs/captureT.png"), SDL_FreeSurface };
            _checked_tile = { IMG_Load("../imgs/checkedT.png"), SDL_FreeSurface };
            _promote_shade = { IMG_Load("../imgs/promoteB.png"), SDL_FreeSurface };
            _font24 = {TTF_OpenFont("../font.ttf", 24), TTF_CloseFont};
            std::cout << _font24 << std::endl;
            _opt_hover.reset();

        }

        void render_piece(DDDelta::BoardCoor coor);
        void render_tile(DDDelta::BoardCoor coor);
        void render_move(DDDelta::BoardCoor coor);
        void render_capture(DDDelta::BoardCoor coor);
        void render_select(DDDelta::BoardCoor coor);
        void render_promote_selection(DDDelta::BoardCoor coor);
    //        void render_result(DDDelta::E_Result res);
        void player_init();
        void board_init();
        void set_hover(DDDelta::BoardCoor hover_coor);

    private:
        using USurfacePtr = std::unique_ptr<SDL_Surface, std::function<decltype(SDL_FreeSurface)>>;
        const DDDelta::ChessGame* _chess_game;
        std::optional<DDDelta::BoardCoor> _opt_hover;
        std::map<DDDelta::Piece, USurfacePtr> _piece_map;
        USurfacePtr _white_tile;
        USurfacePtr _black_tile;
        USurfacePtr _hover_tile;
        USurfacePtr _select_tile;
        USurfacePtr _move_tile;
        USurfacePtr _capture_tile;
        USurfacePtr _checked_tile;
        USurfacePtr _promote_shade;
        std::unique_ptr<TTF_Font, std::function<decltype(TTF_CloseFont)>> _font24;
};



NAMESPACE_BOBZHENG00_END

#endif //DELTAS_CHESS_GUI_H
