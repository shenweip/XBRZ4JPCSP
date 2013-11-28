#pragma once

#include "../Type/CommonType.h"

inline u8 Convert4To8(u8 v)
{
	// Swizzle bits: 00012345 -> 12345123
	return (v << 4) | (v);
}

inline u8 Convert5To8(u8 v)
{
	// Swizzle bits: 00012345 -> 12345123
	return (v << 3) | (v >> 2);
}

inline u8 Convert6To8(u8 v)
{
	// Swizzle bits: 00123456 -> 12345612
	return (v << 2) | (v >> 4);
}