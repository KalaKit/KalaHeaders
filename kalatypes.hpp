//Copyright(C) 2025 Lost Empire Entertainment
//This header comes with ABSOLUTELY NO WARRANTY.
//This is free code, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

// -----------------------------------------------------------------------------
//  Provides explicit, fixed-size integer and float type aliases.
//  Ensures consistent behavior across platforms (Windows, Linux).
//  Includes constexpr min/max bounds and static assertions for type safety.
// -----------------------------------------------------------------------------

#pragma once
#include <cstdint>
#include <limits>

namespace KalaKit
{
    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::numeric_limits;

    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;

    using f32 = float;

    inline constexpr i8  I8_MIN  = numeric_limits<i8>::min();
    inline constexpr i8  I8_MAX  = numeric_limits<i8>::max();
    inline constexpr u8  U8_MAX  = numeric_limits<u8>::max();

    inline constexpr i16 I16_MIN = numeric_limits<i16>::min();
    inline constexpr i16 I16_MAX = numeric_limits<i16>::max();
    inline constexpr u16 U16_MAX = numeric_limits<u16>::max();

    inline constexpr i32 I32_MIN = numeric_limits<i32>::min();
    inline constexpr i32 I32_MAX = numeric_limits<i32>::max();
    inline constexpr u32 U32_MAX = numeric_limits<u32>::max();

    inline constexpr f32 F32_MIN     = numeric_limits<f32>::lowest();
    inline constexpr f32 F32_MAX     = numeric_limits<f32>::max();
    inline constexpr f32 F32_EPSILON = numeric_limits<f32>::epsilon();

    //static_assert sanity checks (compile-time)
    static_assert(sizeof(i8)  == 1);
    static_assert(sizeof(i16) == 2);
    static_assert(sizeof(i32) == 4);

    static_assert(sizeof(u8)  == 1);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(u32) == 4);

    static_assert(sizeof(f32) == 4);
}