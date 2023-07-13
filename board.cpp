#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
    _up_board(std::make_unique<ChessBoard>()),_up_movement_map(std::make_unique<MovementMap>()), _black_lost_val(0), _white_lost_val(0) {
    this->set_up();
}

void Board::set_up() {
    this->get_piece({ 1, 1 }) = WhiteRook;
    this->get_piece({ 2, 1 }) = WhiteKnight;
    this->get_piece({ 3, 1 }) = WhiteBishop;
    this->get_piece({ 4, 1 }) = WhiteQueen;
    this->get_piece({ 5, 1 }) = WhiteKing;
    this->get_piece({ 6, 1 }) = WhiteBishop;
    this->get_piece({ 7, 1 }) = WhiteKnight;
    this->get_piece({ 8, 1 }) = WhiteRook;
    this->get_piece({ 1, 8 }) = BlackRook;
    this->get_piece({ 2, 8 }) = BlackKnight;
    this->get_piece({ 3, 8 }) = BlackBishop;
    this->get_piece({ 4, 8 }) = BlackQueen;
    this->get_piece({ 5, 8 }) = BlackKing;
    this->get_piece({ 6, 8 }) = BlackBishop;
    this->get_piece({ 7, 8 }) = BlackKnight;
    this->get_piece({ 8, 8 }) = BlackRook;

    for (std::uint8_t i : std::views::iota(1, 9)) {
        this->get_piece(i, 2 ) = WhitePawn;
        this->get_piece(i, 7) = BlackPawn;
    }

    for (std::uint8_t i : stdvw::iota(1,9))
        for (std::uint8_t j : stdvw::iota(3, 7))
            this->get_piece(i, j) = Empty;
}

void Board::reset() {
    this->set_up();
    this->_white_lost_val = 0;
    this->_black_lost_val = 0;
    this->_captured_white.clear();
    this->_captured_white.clear();
}

PossibleMovement Board::_pawn_move(BoardCoor co) const {
    std::vector<BoardCoor> sqrs;
    Piece curr = this->get_piece(co);
    BoardCoor new_co;

    new_co = co + BoardCoor(0, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co) == Empty)
        sqrs.push_back(new_co);

    new_co = co + BoardCoor(1, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co).color == ~curr.color)
        sqrs.push_back(new_co);

    new_co = co + BoardCoor(-1, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co).color == ~curr.color)
        sqrs.push_back(new_co);

    new_co = co + BoardCoor(0, curr.color == E_Color::White ? 2 : -2);
    if (co.x == (curr.color == E_Color::White ? 2 : 7) &&
        this->get_piece(new_co) == Empty &&
        this->get_piece(co + BoardCoor(0, curr.color == E_Color::White ? 1 : -1)) == Empty)
        sqrs.push_back(new_co);


}

NAMESPACE_DDDELTA_END