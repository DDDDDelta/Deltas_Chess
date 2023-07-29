#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
        _board {}, _black_lost_val(0), _white_lost_val(0), _in_check(false),
        _white_king_moved(false), _black_king_moved(false) {
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

    for (std::uint8_t i : stdvw::iota(1, 9)) {
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


auto Board::_adaptor_leave_same_color_with(E_Color color) const {
    return stdvw::filter([this, color](BoardCoor coor) { return this->get_piece(coor).color == color; });
}


auto Board::_adaptor_leave_diff_color_with(E_Color color) const {
    return stdvw::filter([this, color](BoardCoor coor) { return this->get_piece(coor).color != color; });
}


void Board::_diagnal_move(BoardCoor co,
                          std::vector<PieceMove>& moves,
                          std::vector<PieceMove>& captures,
                          std::vector<PieceMove>& protects) const {
    E_Color curr_color = this->get_piece(co).color;

    auto insert_move = [this, &moves, &captures, &protects, curr_color](BoardCoor co) mutable {
            if (!co.on_board()) {
                return false;
            } else if (this->get_piece(co).color == curr_color) {
                protects.emplace_back(co, E_UniqueAction::None);
                return false;
            } else if (this->get_piece(co).color == E_Color::Neither) {
                moves.emplace_back(co, E_UniqueAction::None);
                return true;
            } else {
                captures.emplace_back(co, E_UniqueAction::None);
                return false;
            }
    };

    BoardCoor
    curr = co;    while (insert_move(curr += Vec2(1,1)));
    curr = co;    while (insert_move(curr += Vec2(1,-1)));
    curr = co;    while (insert_move(curr += Vec2(-1,-1)));
    curr = co;    while (insert_move(curr += Vec2(-1,1)));
}


void Board::_linear_move(BoardCoor co,
                         std::vector<PieceMove>& moves,
                         std::vector<PieceMove>& captures,
                         std::vector<PieceMove>& protects) const {
    E_Color curr_color = this->get_piece(co).color;

    auto insert_move = [this, &moves, &captures, &protects, curr_color](BoardCoor co) mutable {
        if (!co.on_board()) {
            return false;
        } else if (this->get_piece(co).color == curr_color) {
            protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else if (this->get_piece(co).color == E_Color::Neither) {
            moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else {
            captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;     while (insert_move(curr += Vec2(1,0)));
    curr = co;     while (insert_move(curr += Vec2(-1,0)));
    curr = co;     while (insert_move(curr += Vec2(0,1)));
    curr = co;     while (insert_move(curr += Vec2(0,-1)));
}


PossibleMovement Board::_pawn_move(BoardCoor co) const {
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

    if (new_co.on_board() && this->get_piece(new_co).color == curr.color)
        protects.emplace_back(new_co,
                              new_co.x == 8 || new_co.x == 1 ?
                              E_UniqueAction::Promote :
                              E_UniqueAction::None);

    new_co = co + BoardCoor(-1, curr.color == E_Color::White ? 1 : -1);
    if (new_co.on_board() && this->get_piece(new_co).color == ~curr.color)
        captures.emplace_back(new_co,
                          new_co.x == 8 || new_co.x == 1 ?
                          E_UniqueAction::Promote :
                          E_UniqueAction::None);

    if (new_co.on_board() && this->get_piece(new_co).color == curr.color)
        protects.emplace_back(new_co,
                              new_co.x == 8 || new_co.x == 1 ?
                              E_UniqueAction::Promote :
                              E_UniqueAction::None);

    new_co = co + BoardCoor(0, curr.color == E_Color::White ? 2 : -2);
    if (co.x == (curr.color == E_Color::White ? 2 : 7) &&
        !this->get_piece(new_co) &&
        !this->get_piece(co + BoardCoor(0, curr.color == E_Color::White ? 1 : -1)))
        moves.emplace_back(new_co, E_UniqueAction::None);

    return { std::move(moves), std::move(captures), std::move(protects) };
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
PossibleMovement Board::_king_move(BoardCoor co) const {
    using BC = BoardCoor;

    Piece curr = this->get_piece(co);
    auto move_list = {
            co + Vec2(1, 0),  co + Vec2(1, 1),  co + Vec2(1,-1),
            co + Vec2(0, 1),                         co + Vec2(0, -1),
            co + Vec2(-1, 0), co + Vec2(-1, 1), co + Vec2(-1,-1)
    };

    auto possible_moves = move_list | stdvw::filter([this, curr](BC coor) {
        return coor.on_board() && this->get_piece(coor).color != curr.color;
    });

    auto opponent_pieces = this->_board
            | stdvw::join
            | stdvw::filter([curr](auto& pnm) { return pnm.piece.color == ~curr.color; });



    auto protects_range = move_list | stdvw::filter([this, curr](BC coor) {
        return coor.on_board() && this->get_piece(coor).color == curr.color;
    }) | _adaptor_to_piecemove;

    auto captures_range = possible_moves
            | stdvw::filter([this, curr](BC coor) {
                return this->get_piece(coor).color == ~curr.color;
            })
            | stdvw::filter([&opponent_pieces](BC coor) {
                // PossibleMovement
                auto opponent_movemap = opponent_pieces |
                        stdvw::transform([](auto& pnm) {
                            return *pnm.opt_movement;
                        });
                return stdrng::none_of(opponent_movemap, [coor](const PossibleMovement& pwm) {
                    return stdrng::none_of(pwm.protects, [coor](PieceMove protecting) {
                        return protecting.coor == coor;
                    });
                });
            })
            | _adaptor_to_piecemove;

    auto pred_not_attacked = [this, &opponent_pieces, curr](BC coor) {
        return stdrng::none_of(opponent_pieces, [this, coor, curr](const PieceWithMove& pwm) {
            bool attacked_by_pawn;
            if (curr.color == E_Color::Black) {
                BC top_left = coor + Vec2(-1, 1);
                BC top_right = coor + Vec2(1, 1);
                attacked_by_pawn =
                        !top_left.on_board() ? false : this->get_piece(top_left) == WhitePawn ||
                                                       !top_right.on_board() ? false : this->get_piece(top_left) == WhitePawn;
                // never call get_piece when !on_board()
            } else {
                BC bottom_left = coor + Vec2(-1, -1);
                BC bottom_right = coor + Vec2(1, -1);
                attacked_by_pawn =
                        !bottom_left.on_board() ? false : this->get_piece(bottom_left) == BlackPawn ||
                                                          !bottom_right.on_board() ? false : this->get_piece(bottom_left) == BlackPawn;
                // same as above
            }
            return !attacked_by_pawn &&
                   stdrng::none_of(pwm.opt_movement->moves, [coor](PieceMove moving) {
                       return moving.coor == coor;
                   });
        });
    };

    auto moves_range = possible_moves
            | stdvw::filter([this](BC curr) {
                return !this->get_piece(curr);
            })
            | stdvw::filter(pred_not_attacked)
            | _adaptor_to_piecemove;

    bool king_moved = curr.color == E_Color::White ? this->_white_king_moved : this->_black_king_moved;
    if (!king_moved) {
        if ()
    }

    return { { moves_range.begin(), moves_range.end() },
             { captures_range.begin(), captures_range.end() },
             { protects_range.begin(), protects_range.end() }};
}

PossibleMovement Board::_knight_move(BoardCoor co) const {
    using BC = BoardCoor;

    Piece curr = this->get_piece(co);
    auto move_list = {
            co + Vec2(-1, 2), co + Vec2(1, 2),
            co + Vec2(-2, 1), co + Vec2(2, 1),
            co + Vec2(-2, -1), co + Vec2(2, -1),
            co + Vec2(-1, -2), co + Vec2(1, -2)
    };

    auto possible_move = move_list
            | _adaptor_leave_on_board
            | this->_adaptor_leave_diff_color_with(curr.color);

    auto protects_range = move_list | this->_adaptor_leave_same_color_with(curr.color);

    auto moves_range = possible_move | this->_adaptor_leave_same_color_with(E_Color::Neither);

    auto captures_range = possible_move | this->_adaptor_leave_same_color_with(~curr.color);

    return { { moves_range.begin(), moves_range.end() },
             { captures_range.begin(), captures_range.end() },
             { protects_range.begin(), protects_range.end() }};
}



PossibleMovement Board::_bishop_move(BoardCoor co) const {
    PossibleMovement ret;
    this->_diagnal_move(co, ret.moves, ret.captures, ret.protects);
    return std::move(ret);
}

PossibleMovement Board::_rook_move(BoardCoor co) const {
    PossibleMovement ret;
    this->_linear_move(co, ret.moves, ret.captures, ret.protects);
    return std::move(ret);
}

PossibleMovement Board::_queen_move(BoardCoor co) const {
    PossibleMovement ret;
    this->_linear_move(co, ret.moves, ret.captures, ret.protects);
    this->_diagnal_move(co, ret.moves, ret.captures, ret.protects);
    return std::move(ret);
}

NAMESPACE_DDDELTA_END