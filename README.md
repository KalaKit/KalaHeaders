# KalaHeaders

[![License](https://img.shields.io/badge/license-Zlib-blue)](LICENSE.md)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-brightgreen)

KalaHeaders is a collection of lightweight, header scripts aimed at C++ 20 for various needs. Each header can be used independently from each other and does not depend on anything other than what is included in the header itself. 

All headers are cross-platform (Windows and Unix) and work out of the box. Simply drop any header to your C++20 project and use it as if it was your own source code for your project.

---

# Headers in this repository

## kalatypes.hpp

Provides fixed-size, memory-safe primitive types for cross-platform math, logic, and data layout.
Ensures consistent behavior across platforms (Windows, Linux).
Includes constexpr min/max bounds and static assertions for type safety.
