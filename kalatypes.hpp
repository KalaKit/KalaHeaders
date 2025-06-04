//Copyright(C) 2025 Lost Empire Entertainment
//This header comes with ABSOLUTELY NO WARRANTY.
//This is free code, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

// -----------------------------------------------------------------------------
//  Provides fixed-size, memory-safe primitive types for cross-platform math, logic, and data layout.
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
    using std::suze_t;
    using std::ptrdiff_t;
    using std::numeric_limits;
	
    //
    // definitions
    //
	
    using c8  = char;       //roman letters and arabic numbers
    using c16 = char16_t;   //international letters and symbols
    using c32 = char32_t;   //emojis

    using i8  = int8_t;     //-128 to 127
    using i16 = int16_t;    //-32,768 to 32,767
    using i32 = int32_t;    //-2,147,483,648 to 2,147,483,647

    using u8  = uint8_t;    //0 - 255
    using u16 = uint16_t;   //0 - 65,535
    using u32 = uint32_t;   //0 to 4,294,967,295

    using usize = size_t;
    using isize = ptrdiff_t;

    using f32 = float;
	
    using b8 = u8;          //0 or 1
	
    //
    // min-max sizes
    //

    inline constexpr i8  I8_MIN  = numeric_limits<i8>::min();
    inline constexpr i8  I8_MAX  = numeric_limits<i8>::max();
    inline constexpr u8  U8_MIN  = 0;
    inline constexpr u8  U8_MAX  = numeric_limits<u8>::max();

    inline constexpr i16 I16_MIN = numeric_limits<i16>::min();
    inline constexpr i16 I16_MAX = numeric_limits<i16>::max();
    inline constexpr u16 U16_MIN = 0;
    inline constexpr u16 U16_MAX = numeric_limits<u16>::max();

    inline constexpr i32 I32_MIN = numeric_limits<i32>::min();
    inline constexpr i32 I32_MAX = numeric_limits<i32>::max();
    inline constexpr u32 U32_MIN = 0;
    inline constexpr u32 U32_MAX = numeric_limits<u32>::max();
	
    inline constexpr isize ISIZE_MIN = numeric_limits<isize>::min();
    inline constexpr isize ISIZE_MAX = numeric_limits<isize>::max();
    inline constexpr usize USIZE_MIN = 0;
    inline constexpr usize USIZE_MAX = numeric_limits<usize>::max();

    inline constexpr f32 F32_MIN     = numeric_limits<f32>::lowest();
    inline constexpr f32 F32_MAX     = numeric_limits<f32>::max();
    inline constexpr f32 F32_EPSILON = numeric_limits<f32>::epsilon();
	
    enum class b8state : u8
    {
        false_value = 0,
        true_value = 1,
        invalid     = 2
    };
    inline constexpr bool is_known_b8(b8state v) { return v <= b8state::invalid; }
    #define b8_false   b8state::false_value
    #define b8_true    b8state::true_value
    #define b8_invalid b8state::invalid
    #define check_known_b8(v) do { if (!is_known_b8(v)) __debugbreak(); } while(0)
		
    //
    // static_assert sanity checks (compile-time)
    //
	
    static_assert(sizeof(i8)  == 1);
    static_assert(sizeof(i16) == 2);
    static_assert(sizeof(i32) == 4);

    static_assert(sizeof(u8)  == 1);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(u32) == 4);

    static_assert(sizeof(usize) == sizeof(void*));
    static_assert(sizeof(isize) == sizeof(void*));

    static_assert(sizeof(f32) == 4);
	
    static_assert(sizeof(b8) == 1);
	
    static_assert(sizeof(c8)  == 1);
    static_assert(sizeof(c16) == 2);
    static_assert(sizeof(c32) == 4);
}

//
// TODO: future math and utility types to extend KalaKit base types
//

// Vector types
// using vec2f = ...;   // 2D float vector (x, y)
// using vec3f = ...;   // 3D float vector (x, y, z)
// using vec4f = ...;   // 4D float vector (x, y, z, w)
// using vec2i = ...;   // 2D integer vector
// using vec3i = ...;   // 3D integer vector
// using vec4b8 = ...;  // 4D packed boolean vector (useful for flags)

// Matrix types
// using mat2 = ...;    // 2x2 float matrix
// using mat3 = ...;    // 3x3 float matrix
// using mat4 = ...;    // 4x4 float matrix

// Quaternion and rotation
// using quat = ...;    // quaternion for rotation (x, y, z, w)
// using euler = ...;   // euler angles (pitch, yaw, roll)
// using radians = f32; // semantic angle type

// Color types
// using rgba8 = u32;   // packed 8-bit RGBA
// using coloru32 = u32;// alternate name for packed color

// Rectangles and boxes
// using rectf = ...;   // 2D rectangle with position + size
// using box3f = ...;   // 3D AABB or bounding box

// Math constants
// inline constexpr f32 f32_pi  = 3.141593f;
// inline constexpr f32 f32_tau = 6.283185f;
// inline constexpr f32 deg2rad     = f32_pi / 180.0f;
// inline constexpr f32 rad2deg     = 180.0f / f32_pi;

// Bit/byte utilities
// inline constexpr usize bits_per_byte  = 8;
// inline constexpr usize bits_per_u8    = 8;
// inline constexpr usize bits_per_u32   = 32;
