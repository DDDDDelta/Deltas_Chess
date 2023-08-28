#ifndef DELTAS_CHESS_GUI_H
#define DELTAS_CHESS_GUI_H

#include <map>
#include <utility>
#include <memory>
#include <functional>
#include <cassert>

#include "chess_game.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include "code_utils.inc"

USING_DDDELTA_INTDEF
NAMESPACE_BOBZHENG00_START

// first->white side, second->black side
template <typename T>
struct promotion_pair : std::pair<T, T> {
    inline constexpr promotion_pair(T first, T second) : std::pair<T, T>(first, second) {}
    inline constexpr T& operator [](i32 row) { return row==8 ? this->first : this->second; }
    inline constexpr const T& operator [](i32 row) const { return row==8 ? this->first : this->second; }
};

template <i32 Distance> requires (Distance <= 8 && Distance >= 1)
inline constexpr promotion_pair<i32> y_position { 9 - Distance, 0 + Distance };
inline constexpr promotion_pair<DDDelta::E_Color> piece_color { DDDelta::E_Color::White , DDDelta::E_Color::Black };


template <typename T>
struct result_pair : std::pair<T, T> {
    inline constexpr result_pair(T first, T second) : std::pair<T, T>(first, second) {}
    inline constexpr T& operator [](DDDelta::E_Result res) { return res==DDDelta::E_Result::BLACK_WIN ? this->first : this->second; }
    inline constexpr const T& operator [](DDDelta::E_Result res) const { return res==DDDelta::E_Result::BLACK_WIN ? this->first : this->second; }
};

inline constexpr result_pair<SDL_Color> result_color_black { {124, 252, 0, 255}, {255, 0, 0, 255} };
inline constexpr result_pair<SDL_Color> result_color_white { {255, 0, 0, 255}, {124, 252, 0, 255} };
inline constexpr result_pair<const char*> result_text { "Black Wins" , "White Wins" };


class GUI {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;

        explicit GUI(const DDDelta::ChessGame* chess_game);

        void render_piece(DDDelta::BoardCoor coor);
        void render_tile(DDDelta::BoardCoor coor);
        void render_move(DDDelta::BoardCoor coor);
        void render_capture(DDDelta::BoardCoor coor);
        void render_select(DDDelta::BoardCoor coor);
        void render_possible_moves(const std::shared_ptr<const DDDelta::PossibleMovement>& movement, std::optional<DDDelta::BoardCoor>);
        void render_promote_selection(DDDelta::BoardCoor coor);
        void render_text(const std::string& text, SDL_Color text_color, i32 width, i32 height, _TTF_Font* font);
        void render_result(DDDelta::E_Result res);
        void player_init();
        void board_init();
        void set_hover(DDDelta::BoardCoor hover_coor);

    private:
        using UTexturePtr = std::unique_ptr<SDL_Texture, std::function<decltype(SDL_DestroyTexture)>>;
        using UFontPtr = std::unique_ptr<TTF_Font, std::function<decltype(TTF_CloseFont)>>;
        const DDDelta::ChessGame* _chess_game;
        std::optional<DDDelta::BoardCoor> _opt_hover;
        std::map<DDDelta::Piece, UTexturePtr> _texture_map;

        UTexturePtr _move_texture;
        UTexturePtr _white_texture;
        UTexturePtr _black_texture;
        UTexturePtr _hover_texture;
        UTexturePtr _select_texture;
        UTexturePtr _capture_texture;
        UTexturePtr _checked_texture;
        UTexturePtr _promote_texture;
        UFontPtr _font24;


};



NAMESPACE_BOBZHENG00_END

#endif //DELTAS_CHESS_GUI_H
