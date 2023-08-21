#include "chess_game.h"
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

    bool is_legal = it_move != all_legal_move_rng.end();

    // if illegal move
    if (!is_legal) {
        LOG_TO_STDOUT("illegal execution");
        this->_selected = constant::INVALID_COOR;
        return nullopt;
    }

    // if is a promotion
    if (it_move->unique_action == E_UniqueAction::Promote) {
        LOG_TO_STDOUT("promoting");
        BoardCoor temp = this->_selected;
        this->_selected = constant::INVALID_COOR;
        throw throwable::pawn_promote(&this->_board, target_coor, temp);
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
pawn_promote::pawn_promote(Board* board, BoardCoor target, BoardCoor original) :
    _p_board(board), _target(target), _original(original), _color(board->get_piece(this->_original)->color) {
    assert_on_board_coor(this->_original);

    LOG_TO_STDOUT("throwable constructed");
    board->_get_piece_ref(original).reset();
}


bool pawn_promote::select_promotion(BoardCoor selection) {
    assert(!this->_used_flag);

    LOG_TO_STDOUT("selecting promotion");
    E_PieceType type;

    if (selection.x != this->_target.x)
        return false;

    if (selection.y == nth_from_last_rank<8>[this->_color])
        type = E_PieceType::Queen;
    else if (selection.y == nth_from_last_rank<7>[this->_color])
        type = E_PieceType::Knight;
    else if (selection.y == nth_from_last_rank<6>[this->_color])
        type = E_PieceType::Rook;
    else if (selection.y == nth_from_last_rank<5>[this->_color])
        type = E_PieceType::Bishop;
    else
        return false;

    this->_p_board->_get_piece_ref(this->_target) = Piece(this->_color, type);

    this->_used_flag = true;
    return true;
}


game_end::game_end(BoardCoor in_check, E_Result result) : king_pos(in_check), res(result) {}
} // namespace throwable
NAMESPACE_DDDELTA_END

