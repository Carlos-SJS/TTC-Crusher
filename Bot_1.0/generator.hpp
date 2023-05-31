#pragma once

#include <vector>

#include "util.hpp"

using namespace std;
#define moves(data) ((data>>10)>3)
#define captures(data, side) ((side==0?((data>>2)&7):((data>>5)&7))!=5&&(data>>10)>3)
#define mcount(move_set) (__builtin_popcount(move_set))

namespace generator{
    void get_moves(board &b, u16 side, vector<u64>& moves);

    u16 pawn_moves(int b, u16 dir, u16 other, bool m);
    u16 knight_moves(u16 b, u16 other, bool m);
    u16 bishop_moves(u16 b, u16 other, bool m);
    u16 rook_moves(u16 b, u16 other, bool m);

    u16 pawn_captures(u16 b, u16 dir, u16 other);
    u16 knight_captures(u16 b, u16 other);
    u16 bishop_captures(u16 b, u16 other, u16 blockers);
    u16 rook_captures(u16 b, u16 other, u16 blockers);
};