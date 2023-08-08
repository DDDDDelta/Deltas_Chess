#pragma once
#define DELTAS_CHESS_CODE_UTILS_INC


// symbols
#define NAMESPACE_DDDELTA_START namespace DDDelta {
#define NAMESPACE_DDDELTA_END }

#define NAMESPACE_BOBZHENG00_START namespace BobZheng00 {
#define NAMESPACE_BOBZHENG00_END }


// macro functions
#ifdef __GNUC__
#define UNREACHABLE() __builtin_unreachable()
#elifdef _MSC_VER
#define UNREACHABLE() __assume(false);
#endif

#ifdef __ASSERT_H_
#define assert_on_board_xy(x, y) (assert(x <= 8 && x >= 1 && y <= 8 && y >= 1))
#define assert_on_board_coor(coor) (assert(coor.x <= 8 && coor.x >= 1 && coor.y <= 8 && coor.y >= 1))
#endif // __ASSERT_H_

#define assert_on_board_xy(x, y)
// types
NAMESPACE_DDDELTA_START
#if defined _GLIBCXX_CSTDINT
using u8  = std::uint8_t;
using i8  = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
#endif // defined _GLIBCXX_CSTDINT || defined _GLIBCXX_CSTDDEF
NAMESPACE_DDDELTA_END

#define USING_DDDELTA_INTDEF \
using DDDelta::ui8, DDDelta::i8, DDDelta::ui16, DDDelta::i16, DDDelta::ui32, DDDelta::i32, DDDelta::ui64, DDDelta::i64

#ifdef _GLIBCXX_CHRONO
namespace stdchr = std::chrono;
#endif // _GLIBCXX_CHRONO
#if defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES
namespace stdrng = std::ranges;
#endif // defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES
#ifdef _GLIBCXX_RANGES
namespace stdvw = std::views;
#endif // _GLIBCXX_RANGES