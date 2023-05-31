#include <iostream>

#include "generator.hpp"
#include "move_data.hpp"

u16 board_mask = 65535;

int board_to_sq(int x){
    uint64_t y = x^(x-1);

    uint8_t z = (debruijn*y) >> 58;

    return lookup[z];
}

void print_board_(u16 b){
    vector<vector<char>> a(4, vector<char>(4, '.'));
    int x, y;
    x = 0, y = 0;
    cout << b << '\n';
    while(b){
        if(b&1) a[y][x] = 'X';
        x++;
        if(x > 3) x = 0, y++;
        b>>=1;
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            cout << a[i][j];
        cout << '\n';
    }
}


void generator::get_moves(board &b, u16 side, vector<u64>& moves){
    u64 bd = (side==0?b.white:b.black);
    u64 other_board = (side?b.white:b.black);

    u16 other = (other_board&board_mask)|((other_board>>16)&board_mask)|((other_board>>32)&board_mask)|((other_board>>48)&board_mask);
    u16 all = other|(bd&board_mask)|((bd>>16)&board_mask)|((bd>>32)&board_mask)|((bd>>48)&board_mask);
    
    u16 pawn_dir = (b.data>>side)&1;

    //print_board_(other);
    //print_board_(all);
    //cout << "Pawn side: " << pawn_dir << '\n';

    bool mov = moves(b.data);

    u16 pawn   = pawn_moves(bd&board_mask, pawn_dir, all, mov);
    u16 knight = knight_moves((bd>>16)&board_mask, all, mov);
    u16 bishop = bishop_moves((bd>>32)&board_mask, all, mov);
    u16 rook   = rook_moves((bd>>48)&board_mask, all, mov);

    if(captures(b.data, side)){
        pawn   |= pawn_captures(bd&board_mask, pawn_dir, other);
        knight |= knight_captures((bd>>16)&board_mask, other);
        bishop |= bishop_captures((bd>>32)&board_mask, other, all);
        rook   |= rook_captures((bd>>48)&board_mask, other, all);
    }

    //cout << "Move count: " << mcount(pawn)+mcount(knight)+mcount(bishop)+mcount(rook) << '\n';

    moves.resize(mcount(pawn)+mcount(knight)+mcount(bishop)+mcount(rook));
    int index = 0;

    while(pawn)
        moves[index] = 0 | ((bd&board_mask)<<16) | ((pawn&(-pawn))<<32), pawn &= ~(pawn&(-pawn)), index++;

    while(bishop)
        moves[index] = 32 | (((bd>>32)&board_mask)<<16) | ((bishop&(-bishop))<<32), bishop &= ~(bishop&(-bishop)), index++;

    while(knight)
        moves[index] = 16 | (((bd>>16)&board_mask)<<16) | ((knight&(-knight))<<32), knight &= ~(knight&(-knight)), index++;

    while(rook)
        moves[index] = 48 | (((bd>>48)&board_mask)<<16) | ((rook&(-rook))<<32), rook &= ~(rook&(-rook)), index++;
    
}

u16 generator::pawn_moves(int b, u16 dir, u16 other, bool m){
    if(b == 0) return board_mask&(~other);
    if(!m) return 0;
    

    /*if(dir){
        print_board_(south_pawn_m[board_to_sq(b)]);
        print_board_(((~other)&board_mask));
    }*/

    if(dir) return south_pawn_m[board_to_sq(b)]&((~other)&board_mask);
    return north_pawn_m[board_to_sq(b)]&((~other)&board_mask);
}

u16 generator::knight_moves(u16 b, u16 other, bool m){
    if(b == 0) return board_mask&(~other);
    if(!m) return 0;

    return knight_m[board_to_sq(b)]&(~other);
}

u16 generator::bishop_moves(u16 b, u16 other, bool m){
    if(b == 0) return board_mask&(~other);
    if(!m) return 0;

    u16 p = board_to_sq(b); 
    u16 h = (((other&bishop_rays[p])*bishop_magics[p])&board_mask)>>(16-2);
    return bishop_m[p][h]&(~other);
}

u16 generator::rook_moves(u16 b, u16 other, bool m){    
    if(b == 0) return board_mask&(~other);
    if(!m) return 0;

    u16 p = board_to_sq(b); 
    u16 h = (((other&rook_rays[p])*rook_magics[p])&board_mask)>>(16-4);
    return rook_m[p][h]&(~other);
}

u16 generator::pawn_captures(u16 b, u16 dir, u16 other){
    if(b == 0) return 0;

    if(dir) return south_pawn_c[board_to_sq(b)]&other;
    return north_pawn_c[board_to_sq(b)]&other;
}

u16 generator::knight_captures(u16 b, u16 other){
    if(b == 0) return 0;

    return knight_m[board_to_sq(b)]&other;
}

u16 generator::bishop_captures(u16 b, u16 other, u16 blockers){
    if(b == 0) return 0;

    u16 p = board_to_sq(b); 
    u16 h = (((blockers&bishop_rays[p])*bishop_magics[p])&board_mask)>>(16-2);
    return bishop_m[p][h]&other;
}

u16 generator::rook_captures(u16 b, u16 other, u16 blockers){
    if(b == 0) return 0;

    u16 p = board_to_sq(b); 
    u16 h = (((blockers&rook_rays[p])*rook_magics[p])&board_mask)>>(16-4);
    return rook_m[p][h]&other;
}