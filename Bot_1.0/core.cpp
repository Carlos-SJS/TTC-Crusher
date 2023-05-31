#include "core.hpp"
#include "generator.hpp"

#include<iostream>
 
PlayerCore::PlayerCore(){
    srand(time(NULL));
}

void print_board(u16 b){
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


int shift[] = {0, 0, 32, 16, 48};
board PlayerCore::vector_to_board(vector<vector<int>> &bd){
    board b;
    b.white = 0;
    b.black = 0;
    b.data = 0;

    u64 p = 1;

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            //print_board(p);
            //cout << bd[i][j] << '\n';
            if(bd[i][j] > 0)
                b.white |= (p<<shift[bd[i][j]]);
            else if(bd[i][j] < 0)
                b.black |= (p<<shift[-bd[i][j]]);
            p<<=1;
        }
    }

    b.data |= this->wpawn_dir;
    b.data |= this->wpawn_dir<<1;
    b.data |= this->wcaptures<<2;
    b.data |= this->bcaptures<<5;
    b.data |= this->move_count << 10;

    return b;
} 

vector<vector<int>> PlayerCore::board_to_vector(board &b){
    vector<vector<int>> bd(4, vector<int>(4, 0));

    u64 p = 1;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(b.white&p) bd[i][j] = 1;
            if(b.white&(p<<16)) bd[i][j] = 3;
            if(b.white&(p<<32)) bd[i][j] = 2;
            if(b.white&(p<<48)) bd[i][j] = 4;

            if(b.black&p) bd[i][j] = -1;
            if(b.black&(p<<16)) bd[i][j] = -3;
            if(b.black&(p<<32)) bd[i][j] = -2;
            if(b.black&(p<<48)) bd[i][j] = -4;


            p <<= 1;
        }
    }

    return bd;
}

vector<vector<int>> PlayerCore::getMove(vector<vector<int>> b){
    //cout << "Converting array->board\n";
    board bd = this->vector_to_board(b);

    //print_board(bd.white&65535ll);
    //print_board(bd.black&65535ll);

    vector<u64> moves;
    //cout << "Getting moves\n";

    if(this->bpawn_dir && (bd.black & (15<<12)))
        this -> bpawn_dir ^= 1;
    else if(!this->bpawn_dir && (bd.black & (15)))
        this -> bpawn_dir ^= 1;
    
    if(!(bd.white&65535ll))
        this -> wpawn_dir = this->default_pawnd;

    generator::get_moves(bd, 0, moves);

    //cout << "Applying moves\n";
    //cout << "Moves: " << moves.size() << '\n';

    if(moves.size() == 0){
        cout << "No valid moves found :c\n";
        return b;
    }

    u64 m = moves[rand()%moves.size()];
    bd.white ^= ((m>>16)&65535ll)<<(m&63);
    u64 p = (m>>32)&65535ll;
    bd.white |= p<<(m&63);

    if(bd.black & (p|(p<<16)|(p<<32)|(p<<48)))
        this -> wcaptures++;
    bd.black &= ~(p|(p<<16)|(p<<32)|(p<<48));

    if(this->wpawn_dir && (bd.white & (15<<12)))
        this -> wpawn_dir ^= 1;
    else if(!this->wpawn_dir && (bd.white & (15)))
        this -> wpawn_dir ^= 1;
    
    if(!(bd.black&65535ll))
       this -> bpawn_dir = this->default_pawnd^1;

    print_board(bd.white&65535ll);
    //print_board(bd.black&65535ll);

    //cout << "Converting board->array\n";
    this->move_count++;
    return board_to_vector(bd);
}

void PlayerCore::reset(int color){
    this -> wcaptures = 0;
    this -> bcaptures = 0;

    this -> move_count = 0;

    this -> default_pawnd = 0;
    this -> wpawn_dir = this -> default_pawnd;
    this -> bpawn_dir = this -> default_pawnd^1;
}