#include "new_chess_game.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame(Player&& pwhite, Player&& pblack) :
    player_white(pwhite), player_black(pblack), _board(), _selected(constant::INVALID_COOR),
    _res(E_Result::UNFINISHED), _turn(E_Color::White), _sp_possible_move(nullptr) {}


std::weak_ptr<const PossibleMovement> ChessGame::select_piece(BoardCoor co) {
    assert(this->_selected == constant::INVALID_COOR);

    // if the selection is illegal
    if (!this->get_piece(co) || this->get_piece(co)->color != this->_turn) {
        LOG_TO_STDOUT("illegal selection");
        this->_sp_possible_move.reset();
        return this->_sp_possible_move;
    }

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

    // if illegal move
    if (it_move == all_legal_move_rng.end()) {
        LOG_TO_STDOUT("illegal execution");
        this->_selected = constant::INVALID_COOR;
        return nullopt;
    }

    // if is a promotion
    if (it_move->unique_action == E_UniqueAction::Promote) {
        LOG_TO_STDOUT("promoting");
        BoardCoor temp = this->_selected;
        this->_selected = constant::INVALID_COOR;
        throw throwable::pawn_promote(&this->_board, target_coor, temp, &this->_turn);
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





NAMESPACE_DDDELTA_END

