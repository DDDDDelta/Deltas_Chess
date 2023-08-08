#include "chess_game.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _board(), _selected(constant::INVALID_COOR),
    _res(E_Result::UNFINISHED), _turn(E_Color::White) {}


const std::optional<PossibleMovement>& ChessGame::select_piece(BoardCoor co) {
    assert(this->_selected == constant::INVALID_COOR);

    if (this->_board.get_piece(co) != constant::Empty &&
        this->_board.get_piece(co).color == this->_turn)
        this->_selected = constant::INVALID_COOR;

    return this->_board.get_move(co);
}

/*
 * returns std::nullopt if execution is illegal
 * returns E_UniqueAction according to executed move
 * throws throwable::pawn_promote(this, target_coor) if the move is a pawn promotion
 */
std::optional<E_UniqueAction> ChessGame::execute_move(BoardCoor target_coor) {
    assert(target_coor.on_board());
    assert(this->_selected != constant::INVALID_COOR);

    auto& opt_moves = this->_board.get_move(target_coor);
    if (!opt_moves)
        return std::nullopt;

    Piece selection = this->get_piece(target_coor);

    auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };

    auto it_move = stdrng::find_if(opt_moves->moves, has_move);
    auto it_captures = stdrng::find_if(opt_moves->captures, has_move);

    bool is_legal_move = it_move != opt_moves->moves.begin();
    bool is_legal_capture = it_captures != opt_moves->captures.begin();
    if (!is_legal_move && !is_legal_capture) {
        this->_selected = constant::INVALID_COOR;
        return std::nullopt;
    }

    if (is_legal_move && it_move->unique_action == E_UniqueAction::Promote ||
        is_legal_capture && it_captures->unique_action == E_UniqueAction::Promote)
        throw throwable::pawn_promote(this, target_coor);

    else if (is_legal_capture && it_captures->unique_action == E_UniqueAction::EnPassant)
        this->get_piece(static_cast<std::int8_t> (target_coor.x - this->_selected.x), this->_selected.y) = constant::Empty;

    else if (is_legal_move && it_move->unique_action == E_UniqueAction::LongCastle) {
        std::swap(this->get_piece(1, target_coor.y), this->get_piece(4, target_coor.y));
        this->_king_move[selection.color] = true;

    } else if (is_legal_move && it_move->unique_action == E_UniqueAction::ShortCastle) {
        std::swap(this->get_piece(8, target_coor.y), this->get_piece(7, target_coor.y));
        this->_king_move[selection.color] = true;
    }

    this->get_piece(target_coor) = this->get_piece(this->_selected);
    this->get_piece(this->_selected) = constant::Empty;
    this->_board.remap_move();
    this->_turn = ~this->_turn;
    return E_UniqueAction::None;
}




namespace throwable {
    pawn_promote::pawn_promote(DDDelta::ChessGame* p_game, BoardCoor co) :
        _p_game(p_game), _co(co), _original(p_game->_selected) {}

    bool pawn_promote::select_promotion(BoardCoor selection) {
        E_PieceType type;
        E_Color color = this->_p_game->get_piece(this->_original).color;

        if (selection.x == this->_co.x) {
            switch (selection.y) {
                case 1:
                case 8:
                    type = E_PieceType::Queen;
                    break;
                case 2:
                case 7:
                    type = E_PieceType::Knight;
                    break;
                case 3:
                case 6:
                    type = E_PieceType::Rook;
                    break;
                case 4:
                case 5:
                    type = E_PieceType::Bishop;
                    break;
                default:
                    return false;
            }
        } else
            return false;

        this->_p_game->get_piece(this->_co) = constant::Empty;
        this->_p_game->get_piece(selection) = Piece(color, type);
        this->_p_game->_selected = constant::INVALID_COOR;
        this->_p_game->_board.remap_move();

        return true;
    }
}
NAMESPACE_DDDELTA_END

