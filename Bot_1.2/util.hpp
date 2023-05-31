#pragma once

#ifndef util_hpp
#define util_hpp

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

#define pb push_back

typedef unsigned long long u64;
typedef unsigned long long u32;
typedef unsigned long long u16;
//typedef unsigned int u32;
//typedef unsigned short int u16;

// Moves:
//  Bit 0 - 3 piece to move
//  0 pawn, 1 knight, 2 bishop, 3 rook
//  Bit 16 - 31 from
//  Bit 32 - 45 to 


const u64 lookup[] = {
    0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63
};

const u64 debruijn = 0x03f79d71b4cb0a89;

struct board{
    // Bit 0-15 pawn, 16 - 31 knight, 32 - 45 bishop, 46 - 31 rook
    u64 white;
    u64 black;

    // Bit 0 white pawn direction (0 up, 1 down), 1 black pawn direction (0 up, 1 down)
    // Bit 2 - 4 white capture count (0-5), 5 - 7 black capture count (0-5)
    // Bit 10 - 15 (move count)
    u16 data;  
};

#endif