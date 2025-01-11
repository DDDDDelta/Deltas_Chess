#include "chess_game.h"

#include <algorithm>
#include <ranges>

#include "code_utils.inc"

NAMESPACE_DDDELTA_START
ChessGame::ChessGame() :
    _turn(E_Color::White), _selected(constant::INVALID_COOR),
    _res(E_Result::UNFINISHED), _board(),
    _up_possible_move(nullptr) {}


const PossibleMovement* ChessGame::select_piece(BoardCoor co) {
    assert(this->_selected == constant::INVALID_COOR);

    // if the selection is illegal
    if (!this->get_piece(co) || this->get_piece(co)->color != this->_turn) {
        this->_up_possible_move.reset();
        return this->_up_possible_move.get();
    }

    this->_up_possible_move.reset(this->_board.get_move(co));
    if (this->_up_possible_move) {
        this->_selected = co;
    }

    return this->_up_possible_move.get();
}


std::optional<E_UniqueAction> ChessGame::execute_move(BoardCoor target_coor) {
    assert(target_coor.on_board());
    assert(this->_selected != constant::INVALID_COOR);

    auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };
    auto moves = { this->_up_possible_move->moves, this->_up_possible_move->captures };
    auto all_legal_move_rng = moves | stdvws::join;
    auto it_move = stdrng::find_if(all_legal_move_rng, has_move);

    // if illegal move
    if (it_move == all_legal_move_rng.end()) {
        this->_selected = constant::INVALID_COOR;
        return nullopt;
    }

    // if is a promotion
    if (it_move->unique_action == E_UniqueAction::Promote) {
        BoardCoor temp = this->_selected;
        this->_selected = constant::INVALID_COOR;
        throw throwable::pawn_promote(&this->_board, target_coor, temp, &this->_turn);
    }

    BoardCoor in_check_king_pos = this->_board.execute_move(this->_selected, *it_move);

    if (this->_board.is_checkmated()) {
        this->_res = to_underlying(this->_turn) ? E_Result::WHITE_WIN : E_Result::BLACK_WIN;
        throw throwable::game_end(in_check_king_pos, this->_res);
    }

    this->_turn = !this->_turn;
    this->_selected = constant::INVALID_COOR; // added 2023/8/18

    return it_move->unique_action;
}




namespace throwable {
pawn_promote::pawn_promote(Board* board, BoardCoor target, BoardCoor original, E_Color* turn) :
    _p_board(board), _target(target), _original(original), _turn(turn) {
    assert_on_board_coor(this->_original);

    board->_get_piece_ref(original).reset();
}


pawn_promote::~pawn_promote() {
    if (!this->_used_flag)
        this->_p_board->_get_piece_ref(this->_original).emplace(*this->_turn, E_PieceType::Pawn);
    else
        *this->_turn = !*this->_turn;
}


bool pawn_promote::select_promotion(BoardCoor selection) {
    assert(!this->_used_flag);

    E_PieceType type;

    if (selection.x != this->_target.x)
        return false;

    if (selection.y == nth_from_last_rank<8>[*this->_turn])
        type = E_PieceType::Queen;
    else if (selection.y == nth_from_last_rank<7>[*this->_turn])
        type = E_PieceType::Knight;
    else if (selection.y == nth_from_last_rank<6>[*this->_turn])
        type = E_PieceType::Rook;
    else if (selection.y == nth_from_last_rank<5>[*this->_turn])
        type = E_PieceType::Bishop;
    else
        return false;

    this->_p_board->_get_piece_ref(this->_target) = Piece(*this->_turn, type);
    this->_used_flag = true;
    return true;
}


game_end::game_end(BoardCoor in_check, E_Result result) : king_pos(in_check), res(result) {}
} // namespace throwable
NAMESPACE_DDDELTA_END

