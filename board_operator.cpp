#include "board_operator.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START


Piece StandardBoardOperator::execute_move(BoardCoor selection, PieceMove piece_move, I_ChessGame* p_game) {
    assert(!this->_p_board->is_empty(selection));
    assert_on_board_coor(selection);

    Piece moving_piece = *this->_p_board->get(selection);
    E_Color color = this->_p_board->get(selection)->color;
    E_UniqueAction action = piece_move.unique_action;
    BoardCoor target = piece_move.coor;

    this->_last_double_pawn_move = constant::INVALID_COOR;

    switch (moving_piece.type) {
    case E_PieceType::King:
    this->_can_castle_short[color] = false;
    this->_can_castle_long[color]  = false;
    this->_king_pos[color] = selection;
    break;
    case E_PieceType::Rook:
    if (selection == BoardCoor(1, nth_from_last_rank<1>[color]))
    this->_can_castle_long[color] = false;
    else if (selection == BoardCoor(8, nth_from_last_rank<1>[color]))
    this->_can_castle_short[color] = false;
    break;
    default:
    break;
    }

    this->_get_piece_ref(target) = this->get_piece(selection);
    switch (action) {
    case E_UniqueAction::None:
    break;
    case E_UniqueAction::DoublePawnPush:
    this->_last_double_pawn_move = target;
    break;
    case E_UniqueAction::ShortCastle:
    std::swap(this->_get_piece_ref(selection + Vec2(1, 0)), this->_get_piece_ref(selection + Vec2(3, 0)));
    break;
    case E_UniqueAction::LongCastle:
    std::swap(this->_get_piece_ref(selection - Vec2(1, 0)), this->_get_piece_ref(selection - Vec2(4, 0)));
    break;
    case E_UniqueAction::EnPassant:
    this->_get_piece_ref(target.x, selection.y).reset();
    break;
    case E_UniqueAction::Promote:
    default:
    UNREACHABLE();
    }

    this->_get_piece_ref(selection).reset();

    if (this->_update_attackers(color).attacker_count() != 0)
    return this->_king_pos[!color];

    return constant::INVALID_COOR;
};
NAMESPACE_DDDELTA_END