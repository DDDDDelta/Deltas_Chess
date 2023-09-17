#ifndef DELTAS_CHESS_CODE_UTILS_INC
#define DELTAS_CHESS_CODE_UTILS_INC
#endif


// symbols
#ifndef CODE_UTILS_SYMBOLS_NAMESPACES
#define CODE_UTILS_SYMBOLS_NAMESPACES
#define NAMESPACE_DDDELTA_START namespace DDDelta {
#define NAMESPACE_DDDELTA_END }
#define NAMESPACE_BOBZHENG00_START namespace BobZheng00 {
#define NAMESPACE_BOBZHENG00_END }
#endif // CODE_UTILS_SYMBOLS_NAMESPACES


#ifndef CODE_UTILS_SYMBOLS_NODISCARD
#define CODE_UTILS_SYMBOLS_NODISCARD
#define NODISCARD [[nodiscard]]
#endif // CODE_UTILS_SYMBOLS_NODISCARD


#ifndef CODE_UTILS_SYMBOLS_FORCEINLINE
#define CODE_UTILS_SYMBOLS_FORCEINLINE
#if defined __GNUC__ || defined __clang__
#define FORCEINLINE inline __attribute__((always_inline))
#elif defined _MSC_VER
#define FORCEINLINE __forceinline
#endif
#endif // CODE_UTILS_SYMBOLS_FORCEINLINE


#if defined _GLIBCXX_OPTIONAL && !defined CODE_UTILS_SYMBOLS_NULLOPT
#define CODE_UTILS_SYMBOLS_NULLOPT
using std::nullopt;
#endif // defined _GLIBCXX_OPTIONAL && defined CODE_UTILS_SYMBOLS_NULLOPT


// macro functions
#ifndef CODE_UTILS_FUNCTIONS_UNREACHABLE
#define CODE_UTILS_FUNCTIONS_UNREACHABLE
#if defined __GNUC__
#define UNREACHABLE() __builtin_unreachable()
#elif defined _MSC_VER
#define UNREACHABLE() __assume(false);
#endif
#endif // CODE_UTILS_FUNCTIONS_UNREACHABLE


#ifndef CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD
#define CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD
#ifdef __ASSERT_H_
#define assert_on_board_xy(x, y) (assert(BoardCoor(x, y).on_board()))
#define assert_on_board_coor(coor) (assert(coor.on_board()))
#endif // __ASSERT_H_
#endif // CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD


#if !defined CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT && defined _GLIBCXX_IOSTREAM
#define CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT
#define LOG_TO_STDOUT(message) std::cout << message << std::endl
#endif // !defined CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT && defined _GLIBCXX_IOSTREAM


#if defined _GLIBCXX_TYPE_TRAITS && __cplusplus <= 202002L && !defined CODE_UTILS_FUNCTIONS_TO_UNDERLYING
#define CODE_UTILS_FUNCTIONS_TO_UNDERLYING
template <class E>
inline constexpr std::underlying_type_t<E> to_underlying(E e) {
    return static_cast<std::underlying_type_t<E>>(e);
}
#endif // defined _GLIBCXX_TYPE_TRAITS && __cplusplus <= 202002L && !defined CODE_UTILS_FUNCTIONS_TO_UNDERLYING


// types
NAMESPACE_DDDELTA_START
#if defined _GLIBCXX_CSTDINT && !defined CODE_UTILS_TYPES_INTDEF
#define CODE_UTILS_TYPES_INTDEF
using u8  = std::uint8_t;
using i8  = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
#define USING_DDDELTA_INTDEF \
using DDDelta::u8, DDDelta::i8, DDDelta::u16, DDDelta::i16, DDDelta::u32, DDDelta::i32, DDDelta::u64, DDDelta::i64;
#endif // defined _GLIBCXX_CSTDINT && !defined CODE_UTILS_TYPES_INTDEF


#if defined _GLIBCXX_CFLOAT && !defined CODE_UTILS_TYPES_FLOATDEF
#define CODE_UTILS_TYPES_FLOATDEF
using f32  = float;
using f64  = double;
using f128 = long double;
#define USING_DDDELTA_FLOATDEF \
using DDDelta::f32, DDDelta::f64, DDDelta::f128;
#endif // defined _GLIBCXX_CFLOAT && !defined CODE_UTILS_TYPES_FLOATDEF
NAMESPACE_DDDELTA_END


#if defined _GLIBCXX_CHRONO && !defined CODE_UTILS_TYPES_STDCHR
#define CODE_UTILS_TYPES_STDCHR
namespace stdchr = std::chrono;
#endif // defined _GLIBCXX_CHRONO && !defined CODE_UTILS_TYPES_STDCHR


#if (defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES) && !defined CODE_UTILS_TYPES_STDRNG
#define CODE_UTILS_TYPES_STDRNG
namespace stdrng = std::ranges;
#endif // (defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES) && !defined CODE_UTILS_TYPES_STDRNG


#if defined _GLIBCXX_RANGES && !defined CODE_UTILS_TYPES_STDVW
#define CODE_UTILS_TYPES_STDVW
namespace stdvw = std::views;
#endif // defined _GLIBCXX_RANGES && !defined CODE_UTILS_TYPES_STDVW