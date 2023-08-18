#include "chess_game.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _board(), _selected(constant::INVALID_COOR),
    _res(E_Result::UNFINISHED), _turn(E_Color::White), _sp_possible_move(nullptr) {}


std::weak_ptr<const PossibleMovement> ChessGame::select_piece(BoardCoor co) {
    assert(this->_selected == constant::INVALID_COOR);

    LOG_TO_STDOUT("piece selected");
    this->_sp_possible_move.reset(this->_board.get_move(co));
    if (this->_sp_possible_move) {
        LOG_TO_STDOUT("legal selection");
        this->_selected = co;
    }

    return this->_sp_possible_move;
}


std::optional<E_UniqueAction> ChessGame::execute_move(BoardCoor target_coor) {
    assert(target_coor.on_board());
    assert(this->_selected != constant::INVALID_COOR);

    LOG_TO_STDOUT("executing move");
    auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };
    auto moves = { this->_sp_possible_move->moves, this->_sp_possible_move->captures };
    auto all_legal_move_rng = moves | stdvw::join;
    auto it_move = stdrng::find_if(all_legal_move_rng, has_move);

    bool is_legal = it_move != all_legal_move_rng.end();

    // if not a legal move
    if (!is_legal) {
        LOG_TO_STDOUT("illegal execution");
        this->_selected = constant::INVALID_COOR;
        return nullopt;
    }

    // if is a promotion
    if (it_move->unique_action == E_UniqueAction::Promote) {
        LOG_TO_STDOUT("promoting");
        this->_selected = constant::INVALID_COOR;
        throw throwable::pawn_promote(this, target_coor);
    }

    BoardCoor in_check_king_pos = this->_board.execute_move(this->_selected, *it_move);

    if (this->_board.is_checkmated()) {
        LOG_TO_STDOUT("checkmated");
        this->_res = to_underlying(this->_turn) ? E_Result::WHITE_WIN : E_Result::BLACK_WIN;
        throw throwable::game_end(in_check_king_pos, this->_res);
    }

    LOG_TO_STDOUT("next turn");
    this->_turn = !this->_turn;
    LOG_TO_STDOUT("resetting selection");
    this->_selected = constant::INVALID_COOR; // added 2023/8/18

    return it_move->unique_action;
}




namespace throwable {
pawn_promote::pawn_promote(DDDelta::ChessGame* p_game, BoardCoor co) :
    _p_game(p_game), _co(co), _original(p_game->_selected) {}


bool pawn_promote::select_promotion(BoardCoor selection) {
    E_PieceType type;
    E_Color color = this->_p_game->get_piece(this->_original)->color;

    if (selection.x != this->_co.x)
        return false;

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


    return true;
}


game_end::game_end(BoardCoor in_check, E_Result result) : king_pos(in_check), res(result) {}
}
NAMESPACE_DDDELTA_END

