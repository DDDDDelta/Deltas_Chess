#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
        _board {}, _movement_map {}, _black_lost_val(0), _white_lost_val(0) {
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

PossibleMovement* Board::_pawn_move(BoardCoor co) const {
    std::vector<PieceMove> moves;
    std::vector<PieceMove> captures;
    std::vector<PieceMove> protects;
    Piece curr = this->get_piece(co);
    BoardCoor new_co;

    new_co = co + BoardCoor(0, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && !this->get_piece(new_co))
        moves.emplace_back(new_co,
                          new_co.x == 8 || new_co.x == 1 ?
                          E_UniqueAction::Promote :
                          E_UniqueAction::None);

    new_co = co + BoardCoor(1, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co).color == ~curr.color)
        captures.emplace_back(new_co,
                          new_co.x == 8 || new_co.x == 1 ?
                          E_UniqueAction::Promote :
                          E_UniqueAction::None);

    new_co = co + BoardCoor(-1, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co).color == ~curr.color)
        captures.emplace_back(new_co,
                          new_co.x == 8 || new_co.x == 1 ?
                          E_UniqueAction::Promote :
                          E_UniqueAction::None);

    new_co = co + BoardCoor(0, curr.color == E_Color::White ? 2 : -2);
    if (co.x == (curr.color == E_Color::White ? 2 : 7) &&
        !this->get_piece(new_co) &&
        !this->get_piece(co + BoardCoor(0, curr.color == E_Color::White ? 1 : -1)))
        moves.emplace_back(new_co, E_UniqueAction::None);

    return new PossibleMovement { std::move(moves), std::move(captures) };
}


/*
 * move_list: 8 squares around co
 * ->  possible_moves: leaves squares on board and non-same color with king
 *
 * move_list
 * ->  protects: leaves squares with same color piece
 *
 * captures: pieces able to capture
 * possible_moves
 * ->  keeps opposite color piece
 *     ->  captures: filters ones protected: no pieces protecting: no protection includes the square
 *
 * moves: squares able to move
 * possible_moves
 * ->  keeps empty
 *     ->  moves: filters ones | no pawns protecting: no protection includes the square
 *                             | no pieces moves to: no moves includes the square
 */

PossibleMovement* Board::_king_move(BoardCoor co) const {
    using BC = BoardCoor;
    Piece curr = this->get_piece(co);
    auto move_list = {
            co + BC(1, 0),  co + BC(1, 1),  co + BC(1,-1),
            co + BC(0, 1),                         co + BC(0, -1),
            co + BC(-1, 0), co + BC(-1, 1), co + BC(-1,-1)
    };

    /*
    std::vector<std::vector<PieceMove>*> under_attack_p;
    for (int i : stdvw::iota(0,8))
        for (int j : stdvw::iota(0,8))
            if (!this->_board[i][j] && this->_board[i][j].color == ~curr.color)
                under_attack_p.push_back(&this->_movement_map[i][j]->captures);
    */

    auto possible_moves = move_list | stdvw::filter([this, curr](BC coor) {
        return coor.on_board() && this->get_piece(coor).color != curr.color;
    });

    auto opponent_movemap = stdvw::iota(0, 64)
                            | stdvw::transform([this, curr](int i) -> PossibleMovement* {
                                if (this->get_board()[i / 8][i % 8].color == ~curr.color)
                                    return this->_movement_map[i / 8][i % 8].get();
                                else
                                    return nullptr;
                            })
                            | stdvw::filter([](auto p) {
                                return static_cast<bool>(p);
                            });

    auto captures_range = possible_moves
                        | stdvw::filter([this, curr](BC coor) {
                            return this->get_piece(coor).color == ~curr.color;
                        })
                        | stdvw::filter([&opponent_movemap](BC coor) {
                            return stdrng::none_of(opponent_movemap, [coor](auto p) {
                                return stdrng::none_of(p->protects, [coor](auto protecting) {
                                    return protecting.coor == coor;
                                });
                            });
                        });

    auto move_range = possible_moves
                    | stdvw::filter([this](BC curr) {
                        return !this->get_piece(curr);
                    })
                    | stdvw::filter([&opponent_movemap](BC coor) {
                        
                    });

}

NAMESPACE_DDDELTA_END