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
	using std::size_t;
	using std::ptrdiff_t;
	using std::numeric_limits;

	//
	// CHARS
	//

	using c8 = char;        //roman letters and arabic numbers
	using c16 = char16_t;   //international letters and symbols
	using c32 = char32_t;   //emojis

	static_assert(sizeof(c8) == 1, "c8 must be 1 byte (char)");
	static_assert(sizeof(c16) == 2, "c16 must be 2 bytes (char16_t)");
	static_assert(sizeof(c32) == 4, "c32 must be 4 bytes (char32_t)");

	//
	// SIGNED INTS
	//

	using i8 = int8_t;      //-128 to 127
	using i16 = int16_t;    //-32,768 to 32,767
	using i32 = int32_t;    //-2,147,483,648 to 2,147,483,647

	inline constexpr i8  I8_MIN = numeric_limits<i8>::min();
	inline constexpr i8  I8_MAX = numeric_limits<i8>::max();
	inline constexpr i16 I16_MIN = numeric_limits<i16>::min();
	inline constexpr i16 I16_MAX = numeric_limits<i16>::max();
	inline constexpr i32 I32_MIN = numeric_limits<i32>::min();
	inline constexpr i32 I32_MAX = numeric_limits<i32>::max();

	static_assert(sizeof(i8) == 1, "i8 must be 1 byte (8 bits)");
	static_assert(sizeof(i16) == 2, "i16 must be 2 bytes (16 bits)");
	static_assert(sizeof(i32) == 4, "i32 must be 4 bytes (32 bits)");

	//
	// UNSIGNED INTS
	//

	using u8 = uint8_t;     //0 - 255
	using u16 = uint16_t;   //0 - 65,535
	using u32 = uint32_t;   //0 to 4,294,967,295

	inline constexpr u8  U8_MIN = 0;
	inline constexpr u8  U8_MAX = numeric_limits<u8>::max();
	inline constexpr u16 U16_MIN = 0;
	inline constexpr u16 U16_MAX = numeric_limits<u16>::max();
	inline constexpr u32 U32_MIN = 0;
	inline constexpr u32 U32_MAX = numeric_limits<u32>::max();

	static_assert(sizeof(u8) == 1, "u8 must be 1 byte (8 bits)");
	static_assert(sizeof(u16) == 2, "u16 must be 2 bytes (16 bits)");
	static_assert(sizeof(u32) == 4, "u32 must be 4 bytes (32 bits)");

	//
	// SIZE
	//

	using usize = size_t;
	using isize = ptrdiff_t;

	inline constexpr isize ISIZE_MIN = numeric_limits<isize>::min();
	inline constexpr isize ISIZE_MAX = numeric_limits<isize>::max();
	inline constexpr usize USIZE_MIN = 0;
	inline constexpr usize USIZE_MAX = numeric_limits<usize>::max();

	static_assert(sizeof(usize) == sizeof(void*), "usize must match pointer size");
	static_assert(sizeof(isize) == sizeof(void*), "isize must match pointer size");

	//
	// FLOAT
	//

	using f32 = float;

	inline constexpr f32 F32_MIN = numeric_limits<f32>::lowest();
	inline constexpr f32 F32_MAX = numeric_limits<f32>::max();
	inline constexpr f32 F32_EPSILON = numeric_limits<f32>::epsilon();

	static_assert(sizeof(f32) == 4, "f32 must be 4 bytes (32-bit float)");

	//
	// BOOL
	//

	using b8 = u8;          //array of 8 1-bit bool states
	using b16 = u16;        //array if 16 1-bit bool states
	using b32 = u32;        //array of 32 1-bit bool states

	static_assert(sizeof(b8) == 1, "b8 must be 1 byte (8 bits)");
	static_assert(sizeof(b16) == 2, "b16 must be 2 bytes (16 bits)");
	static_assert(sizeof(b32) == 4, "b32 must be 4 bytes (32 bits)");
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
