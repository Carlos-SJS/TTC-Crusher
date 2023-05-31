#include <iostream>
#include <fstream>

#include "generator.hpp"
#include "util.hpp"

using namespace std;

#define sq_to_bboard(x,y) (1ll<<((y<<2)+x))

void print_board(u16 b, ofstream& f){
    vector<vector<char>> a(4, vector<char>(4, '.'));
    int x, y;
    x = 0, y = 0;
    f << b << '\n';
    while(b){
        if(b&1) a[y][x] = 'X';
        x++;
        if(x > 3) x = 0, y++;
        b>>=1;
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            f << a[i][j];
        f << '\n';
    }
}

int main(){
    board b;
    
    b.white  = sq_to_bboard(0, 3);
    b.white |= sq_to_bboard(1, 1)<<48;
    b.white |= sq_to_bboard(3, 3)<<16;

    b.black  = sq_to_bboard(1, 2)<<32;
    b.black |= sq_to_bboard(2, 1)<<48;

    ofstream f("log.txt");


    /*print_board(b.black&((1ll<<16)-1));
    cout << "\n";
    print_board((b.black>>16)&((1ll<<16)-1));
    cout << "\n";
    print_board((b.black>>32)&((1ll<<16)-1));
    cout << "\n";
    print_board((b.black>>48)&((1ll<<16)-1));
    cout << "\n";

    while(1){};*/


    b.data = (5<<10);

    vector<u64> moves;
    generator::get_moves(b, 0, moves);

    cout << "found " << moves.size() << " moves\n";

    for(u64 m: moves){
        f << "---------\n";
        print_board((m>>16)&((1<<16)-1), f);
        f << '\n';
        print_board((m>>32)&((1<<16)-1), f);
    }   

    cout << "Successfull execution!\n";
    while(1){};
}