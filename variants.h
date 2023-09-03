#pragma once
#define DELTAS_CHESS_VARIANTS_H

#include <concepts>

#include "pieces.h"
#include "moves.h"
#include "move_factory.h"
#include "board_operator.h"
#include "new_board.h"
#include "chess_game_interface.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
namespace _impl {
inline constexpr BoardCoor valid_coor { 1, 1 };
inline constexpr PieceMove valid_move { valid_coor, E_UniqueAction::None };


template <typename T>
concept has_starting_board = requires {
    { T::starting_position } -> std::same_as<RawBoard>;
};


template <typename T>
concept has_operator_and_factory = requires {
    typename T::Operator;
    typename T::Factory;
};


template <typename Operator>
concept is_valid_operator = requires(Operator o) {
    std::constructible_from<Operator, Board*>;
    { o.execute_move(valid_move, valid_coor) } -> std::same_as<Piece>;
};


template <typename Factory>
concept is_valid_factory = requires(Factory f) {
    std::constructible_from<Factory, const Board&, const E_Color&>;
    { f.get_move(valid_coor) } -> std::same_as<PossibleMovement*>;
};
} // namespace _impl


template <typename T>
concept chess_variant =
    _impl::has_starting_board<T> &&
    _impl::has_operator_and_factory<T> &&
    _impl::is_valid_factory<typename T::Factory> &&
    _impl::is_valid_operator<typename T::Operator>;


namespace variant {
class Standard {
public:
    using Operator = StandardBoardOperator;
    using Factory = StandardMoveFactory;
    static constexpr RawBoard starting_position = standard_starting_position;
};
} // namespace variant
NAMESPACE_DDDELTA_END