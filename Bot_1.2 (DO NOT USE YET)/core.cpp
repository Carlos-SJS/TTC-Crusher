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
    b.data |= this->bpawn_dir<<1;
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

// Evaluate a given position
// How? IDK.
int PlayerCore::evaluate(board &p){
    return 5-wcaptures;
}

// This function should apply the move and do some checks
// That is, if it works c:
board PlayerCore::aply_move(board bd, u64 &move, int player){

    if(player){ // Black (inferior side)
        // Check pawn dirs or something
        if((bd.data&1) && (bd.white&(15<<12)))
            bd.data ^= 1;
        if(!(bd.data&1) && (bd.white&(15)))
            bd.data ^= 1;
        
        if(!(bd.black&bmask))
            bd.data ^= (bd.data&2), bd.data |= ((this->default_pawnd^1)<<1);

        // Apply move to bit board
        bd.black ^= ((move>>16)&bmask) << (move&63);

        u64 nw = (move>>32)&bmask, cp = (nw|(nw<<16)|(nw<<32)|(nw<<48));
        bd.black |= nw<<(move&63);

        if(bd.white & cp)
            bd.data += 32;      

        bd.white &= ~cp;

        // Pawn shit
        if((bd.data&2) && (bd.black&(15<<12)))
            bd.data ^= 2;
        if(!(bd.data&2) && (bd.black&(15)))
            bd.data ^= 2;

        if(!(bd.white&bmask))
            bd.data ^= (bd.data&1), bd.data |= this->default_pawnd;

    }else{ // The same shit but white, so it is a little better
        // Something i guess
        if((bd.data&2) && (bd.black&(15<<12)))
            bd.data ^= 2;
        if(!(bd.data&2) && (bd.black&(15)))
            bd.data ^= 2;
        
        if(!(bd.white&bmask))
            bd.data ^= (bd.data&1), bd.data |= this->default_pawnd;

        // Apply move to bit board
        bd.white ^= ((move>>16)&bmask) << ((move&63));

        u64 nw = (move>>32)&bmask, cp = (nw|(nw<<16)|(nw<<32)|(nw<<48));
        bd.white |= nw<<(move&63);

        if(bd.black & cp)
            bd.data += 4;      

        bd.black &= ~cp;

        // Pawn shit
        if((bd.data&1) && (bd.white&(15<<12)))
            bd.data ^= 1;
        if(!(bd.data&1) && (bd.white&(15)))
            bd.data ^= 1;

        if(!(bd.black&bmask))
            bd.data ^= (bd.data&2), bd.data |= ((this->default_pawnd^1)<<1);
    }
    
    bd.data += 1ll<<10;
    return bd;
}

int PlayerCore::alpha_beta(board &bd, int depth, int alpha, int beta, int player){
    if((double)(clock() - start)/CLOCKS_PER_SEC > MAX_TIME)
        return -inf;

    //cout << alpha << " " << beta << " " << depth << " " << player << "\n";
    if((bd.data>>10) > 140)
        return 0;

    if(depth == 0){ // Should also check if node is terminal (someone won, or draw and shit)
        // Should return heuristic value *w*
        return PlayerCore::evaluate(bd);
    }
    
    vector<u64> moves;
    generator::get_moves(bd, player, moves);
    //cout << moves.size() << '\n';

    u64 white = bd.white&bmask|(bd.white>>16)&bmask|(bd.white>>32)&bmask|(bd.white>>48)&bmask;
    u64 black = bd.black&bmask|(bd.black>>16)&bmask|(bd.black>>32)&bmask|(bd.black>>48)&bmask;

    if(!player){ // Maximize
        if(check_win(white)){
            //cout << "reached w win\n";
            return inf;
        }
        if(check_win(black)){
            //cout << "reached b win\n";
            return -inf;
        }
        
        if(moves.size() == 0){
            //cout << "No valid moves??\n";
            return -inf;
        }

        int v = -inf;
        
        for(u64 m: moves){
            v = max(v, this->alpha_beta(this->aply_move(bd, m, 0), depth-1, alpha, beta, 1));
        
            alpha = max(alpha, v);
            if(v >= beta){
                cut_offs++;
                break;
            }
        }

        return v;
    }else{ // Minimize
        if(check_win(white)){
            //cout << "reached w win\n";
            return inf;
        }
        if(check_win(black)){
            //cout << "reached b win\n";
            return -inf;
        }

        if(moves.size() == 0){
            //cout << "No valid moves??\n";
            return inf;
        }


        int v = inf;

        for(u64 m: moves){
            v = min(v, this->alpha_beta(this->aply_move(bd, m, 1), depth-1, alpha, beta, 0));

            beta = min(beta, v);
            if(v < alpha){
                cut_offs++;
                break;
            }
        }

        return v;
    }
}

// Do alpha-beta prunning shit
u64 PlayerCore::find_move(board bd){
    vector<u64> moves;
    generator::get_moves(bd, 0, moves);

    cut_offs = 0;

    cout << "Move count: " << (bd.data>>10) << '\n';
    cout << "wCaps: " << wcaptures << "(" << ((bd.data>>2)&7) << ")\n";
    cout << "bCaps: " << bcaptures << "(" << ((bd.data>>5)&7) << ")\n";

    if(moves.size() == 0){
        cout << "No moves :c\n";
        return 0;
    }
    
    int best = -inf-1;
    u64 best_move = 0;

    for(int deep=0; deep<=140-move_count && (double)(clock() - start)/CLOCKS_PER_SEC < MAX_TIME; deep++){
        for(u64 m: moves){
            int v = this->alpha_beta(this->aply_move(bd, m, 0), search_deepness, -inf, inf, 1);

            if((double)(clock() - start)/CLOCKS_PER_SEC > MAX_TIME)
                break;

            if(v > best)
                best = v, best_move = m;
        }
    }

    cout << "Expected val: " << best << '\n';
    cout << "Cut offs: " << cut_offs << '\n';

    return best_move;
}

vector<vector<int>> PlayerCore::getMove(vector<vector<int>> b){
    start = clock();

    board bd = this->vector_to_board(b);

    if(this->bpawn_dir && (bd.black & (15<<12)))
        this -> bpawn_dir ^= 1;
    else if(!this->bpawn_dir && (bd.black & (15)))
        this -> bpawn_dir ^= 1;
    
    cout << "Pawn dir: " << (bd.data&1) << '\n';
    
    if(!(bd.white&65535ll))
        this -> wpawn_dir = this->default_pawnd;

    if(bd.white != prev_board)
        bcaptures ++;

    bd = this->vector_to_board(b);

    // Search for best move
    u64 m = this -> find_move(bd);
    //cout << "Move is: " << m << '\n';

    u64 p = (m>>32)&65535ll;
    if(bd.black & (p|(p<<16)|(p<<32)|(p<<48)))
        this -> wcaptures++;
    
    cout << "Pawn dir: " << wpawn_dir << '\n';
    if(this->wpawn_dir && (bd.white & (15<<12)))
        this -> wpawn_dir ^= 1;
    else if(!this->wpawn_dir && (bd.white & (15)))
        this -> wpawn_dir ^= 1;

    //cout << "Pawn dir: " << wpawn_dir << '\n';
    
    if(!(bd.black&65535ll))
       this -> bpawn_dir = this->default_pawnd^1;
    //print_board(bd.white&65535ll);
    //print_board((m>>16)&bmask);
    bd = this->aply_move(bd, m, 0);
    //print_board(bd.white&65535ll);
    //print_board((m>>32)&bmask);


    this->move_count+=2;
    prev_board = bd.white;
    return board_to_vector(bd);
}

void PlayerCore::reset(int color){
    this -> wcaptures = 0;
    this -> bcaptures = 0;
    prev_board = 0;

    if(color == 0)
        this -> move_count = 1;
    else
        this->move_count = 2;

    this -> default_pawnd = 0;
    this -> wpawn_dir = this -> default_pawnd;
    this -> bpawn_dir = this -> default_pawnd^1;
}