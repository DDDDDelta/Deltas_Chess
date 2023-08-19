#include <iostream>
#pragma once
#define DELTAS_CHESS_CODE_UTILS_INC


// symbols
#define NAMESPACE_DDDELTA_START namespace DDDelta {
#define NAMESPACE_DDDELTA_END }

#define NAMESPACE_BOBZHENG00_START namespace BobZheng00 {
#define NAMESPACE_BOBZHENG00_END }

#define NODISCARD [[nodiscard]]

#if defined __GNUC__ || defined __clang__
#define FORCEINLINE inline __attribute__((always_inline))
#elif defined _MSC_VER
#define FORCEINLINE __forceinline
#endif

#ifdef _GLIBCXX_OPTIONAL
using std::nullopt;
#endif // _GLIBCXX_OPTIONAL


// macro functions
#if defined __GNUC__
#define UNREACHABLE() __builtin_unreachable()
#elif defined _MSC_VER
#define UNREACHABLE() __assume(false);
#endif


#ifdef __ASSERT_H_
#define assert_on_board_xy(x, y) (assert(BoardCoor(x, y).on_board()))
#define assert_on_board_coor(coor) (assert(coor.on_board()))
#endif // __ASSERT_H_


#define LOG_TO_STDOUT(message) std::cout << (message) << std::endl

// types
NAMESPACE_DDDELTA_START
#ifdef _GLIBCXX_CSTDINT
using u8  = std::uint8_t;
using i8  = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
#endif // defined _GLIBCXX_CSTDINT

#ifdef _GLIBCXX_CFLOAT
using f32  = float;
using f64  = double;
using f128 = long double;
#endif // _GLIBCXX_CFLOAT

#if defined _GLIBCXX_TYPE_TRAITS && __cplusplus <= 202002L
template <class E>
inline constexpr std::underlying_type_t<E> to_underlying(E e) {
    return static_cast<std::underlying_type_t<E>>(e);
}
#endif // _GLIBCXX_TYPE_TRAITS
NAMESPACE_DDDELTA_END


#define USING_DDDELTA_INTDEF \
using DDDelta::u8, DDDelta::i8, DDDelta::u16, DDDelta::i16, DDDelta::u32, DDDelta::i32, DDDelta::u64, DDDelta::i64;
#define USING_DDDELTA_FLOATDEF \
using DDDelta::f32, DDDelta::f64, DDDelta::f128;


#ifdef _GLIBCXX_CHRONO
namespace stdchr = std::chrono;
#endif // _GLIBCXX_CHRONO


#if defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES
namespace stdrng = std::ranges;
#endif // defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES


#ifdef _GLIBCXX_RANGES
namespace stdvw = std::views;
#endif // _GLIBCXX_RANGES