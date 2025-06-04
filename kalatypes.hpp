//Copyright(C) 2025 Lost Empire Entertainment
//This header comes with ABSOLUTELY NO WARRANTY.
//This is free code, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//This header is a part of the KalaKit KalaHeaders repository: https://github.com/KalaKit/KalaHeaders

// ======================================================================
//  Provides fixed-size, memory-safe primitive types for cross-platform math, logic, and data layout.
//  Ensures consistent behavior across platforms (Windows, Linux).
//  Includes constexpr min/max bounds and static assertions for type safety.
// ======================================================================

#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

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
	using std::is_unsigned_v;
	using std::is_same_v;

	// ======================================================================
	// 
	// SIGNED INT
	// 
	// ======================================================================

	using i8 = int8_t;             //-128 to 127
	using i16 = int16_t;           //-32,768 to 32,767
	using i32 = int32_t;           //-2,147,483,648 to 2,147,483,647

	inline constexpr i8  I8_MIN = numeric_limits<i8>::min();
	inline constexpr i8  I8_MAX = numeric_limits<i8>::max();
	inline constexpr i16 I16_MIN = numeric_limits<i16>::min();
	inline constexpr i16 I16_MAX = numeric_limits<i16>::max();
	inline constexpr i32 I32_MIN = numeric_limits<i32>::min();
	inline constexpr i32 I32_MAX = numeric_limits<i32>::max();

	// ======================================================================
	// 
	// UNSIGNED INT
	// 
	// ======================================================================

	using u8 = uint8_t;            //0 - 255
	using u16 = uint16_t;          //0 - 65,535
	using u32 = uint32_t;          //0 to 4,294,967,295

	inline constexpr u8  U8_MIN = 0;
	inline constexpr u8  U8_MAX = numeric_limits<u8>::max();
	inline constexpr u16 U16_MIN = 0;
	inline constexpr u16 U16_MAX = numeric_limits<u16>::max();
	inline constexpr u32 U32_MIN = 0;
	inline constexpr u32 U32_MAX = numeric_limits<u32>::max();

	// ======================================================================
	// 
	// FLOAT
	// 
	// ======================================================================

	using f32 = float;             //up to 38-digit magnitude, ~7 decimal digits precision

	inline constexpr f32 F32_MIN = numeric_limits<f32>::lowest();
	inline constexpr f32 F32_MAX = numeric_limits<f32>::max();
	inline constexpr f32 F32_EPSILON = numeric_limits<f32>::epsilon();

	using f64 = double;            //up to 308-digit magnitude, ~16 decimal digits precision

	inline constexpr f64 F64_MIN = numeric_limits<f64>::lowest();
	inline constexpr f64 F64_MAX = numeric_limits<f64>::max();
	inline constexpr f64 F64_EPSILON = numeric_limits<f64>::epsilon();

	//
	// SIZE
	//

	using usize = size_t;
	using isize = ptrdiff_t;

	inline constexpr isize ISIZE_MIN = numeric_limits<isize>::min();
	inline constexpr isize ISIZE_MAX = numeric_limits<isize>::max();
	inline constexpr usize USIZE_MIN = 0;
	inline constexpr usize USIZE_MAX = numeric_limits<usize>::max();

	// ======================================================================
	// 
	// BITFIELD BOOL
	// 
	// Provides a tightly packed array of 1-bit boolean flags using a backing integer.
	// Reduces memory usage and padding compared to individual bools (which are 1 byte).
	// Backing type (T) must be unsigned: u8, u16 or u32.
	// 
	// Types:
	//   b8 - 8 booleans into 1 byte
	//   b16 - 16 booleans into 2 bytes
	//   b32 - 32 booleans into 4 bytes
	// 
	// Usage:
	//   b8 flags{};
	//   flags[0] = true;                   //enable bit 0
	//   if (flags[3]) { /*do something*/ } //check bit 3
	// 
	//   //with named indices:
	//   constexpr u8 visible = 0;
	//   constexpr u8 active = 1;
	//   flags[visible] = true;
	//   if (flags[active]) { /*do something*/ }
	// ======================================================================

	template<typename T, u8 NumBits>
	struct BitField
	{
		static_assert(is_unsigned_v<T>);
		T value = 0;

		struct BitRef
		{
			T& value;
			u8 index;

			operator bool() const { return (value >> index) & 1; }

			BitRef& operator=(bool b)
			{
				if (b) value |= (T(1) << index);
				else   value &= ~(T(1) << index);
				return *this;
			}
		};

		constexpr BitRef operator[](u8 index) { return BitRef{ value, index }; }
		constexpr bool   operator[](u8 index) const { return (value >> index) & 1; }
	};

	using b8 = BitField<u8, 8>;    //array of 8 1-bit bool states
	using b16 = BitField<u16, 16>; //array of 16 1-bit bool states
	using b32 = BitField<u32, 32>; //array of 32 1-bit bool states

	// ======================================================================
	// 
	// CHAR
	// 
	// ======================================================================

	using c8 = char;               //roman letters and arabic numbers
	using c16 = char16_t;          //international letters and symbols
	using c32 = char32_t;          //emojis and rare unicode

	// ======================================================================
	// 
	// FIXED LENGTH STRING
	// 
	// Defines compile-time fixed-length strings for safe, bounded text storage.
	// These strings are strongly typed and enforce character encoding and size limits.
	// Overwrites zero-fill unused space to prevent garbage memory.
	// 
	// Types:
	//   s8<N>  - N-length string of c8  (1-byte) characters: ASCII/english text and arabic numbers
	//   s16<N> - N-length string of c16 (2-byte) characters: international alphabets
	//   s32<N> - N-length string of c32 (4-byte) characters: full unicode like emojis
	// 
	// Usage:
	//   s8<8> username = "admin";     //ASCII name (max 8 1-byte c8 chars)
	//   s16<16> label = u"こんにちは"; //japanese (max 16 2-byte c16 chars)
	//   s32<4> icons = U"🌟🔥💧";   //emoji set (max 4 4-byte c32 chars)
	// ======================================================================

	template<typename CharT, usize Length>
	struct FixedString
	{
		static_assert
		(
			is_same_v<CharT, c8>
			|| is_same_v<CharT, c16>
			|| is_same_v<CharT, c32>,
			"FixedString only supports c8, c16, c32"
		);

		CharT data[Length]{};

		/// <summary>
		/// Default constructor (zero-filled)
		/// </summary>
		constexpr FixedString() = default;

		/// <summary>
		/// Construct directly from a string literal
		/// </summary>
		template<usize N>
		constexpr FixedString(const CharT(&literal)[N])
		{
			static_assert(N - 1 <= Length, "Literal is too long for FixedString");
			for (usize i = 0; i < N - 1; ++i) data[i] = literal[i];
		}

		/// <summary>
		/// Overwrite contents using a string literal
		/// </summary>
		template<usize N>
		constexpr FixedString& operator=(const CharT(&literal)[N])
		{
			static_assert(N - 1 <= Length, "Literal too long for FixedString");
			for (usize i = 0; i < N - 1; ++i) data[i] = literal[i];

			//zero-fill the rest
			for (usize i = N - 1; i < Length; ++i) data[i] = 0;

			return *this;
		}

		constexpr CharT& operator[](usize index) { return data[index]; }
		constexpr const CharT& operator[](usize index) const { return data[index]; }

		constexpr usize size() const { return Length; }
		constexpr const CharT* begin() const { return data; }
		constexpr const CharT* end() const { return data + Length; }
	};

	template<usize N> using s8 = FixedString<c8, N>;
	template<usize N> using s16 = FixedString<c16, N>;
	template<usize N> using s32 = FixedString<c32, N>;
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
