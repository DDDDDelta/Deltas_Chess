#include "chess_game.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _board(),
    _res(E_Result::UNFINISHED), _turn(E_Color::White) {}


std::optional<PossibleMovement> ChessGame::select_piece(BoardCoor co) const {
    Piece selected = this->_board.get_piece(co);

    return this->_board.get_move(co);
}


namespace throwable {
    pawn_promote::pawn_promote(DDDelta::ChessGame *p_game, BoardCoor co) :
        _p_game(p_game), _co(co) {}

    bool pawn_promote::select_promotion(DDDelta::E_PieceType type) {
        switch (type) {
            case E_PieceType::Bishop:
                this->_p_game->get_piece(this->_p_game->_selected).color = E_Color::Neither;
                return true;
            case E_PieceType::Knight:
                return true;
            case E_PieceType::Rook:
                return true;
            case E_PieceType::Queen:
                return true;
            default:
                return false;
        }

    }
}
NAMESPACE_DDDELTA_END

