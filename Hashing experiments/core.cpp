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
    b.hash = 0;

    u64 p = 1;

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            //print_board(p);
            //cout << bd[i][j] << '\n';
            if(bd[i][j] > 0){
                b.white |= (p<<shift[bd[i][j]]);
                b.hash ^= pice_hash_code[(bd[i][j]-1)*2][4*i+j];
            }else if(bd[i][j] < 0){
                b.black |= (p<<shift[-bd[i][j]]);
                b.hash ^= pice_hash_code[((-bd[i][j])-1)*2+1][4*i+j];
            }
            p<<=1;
        }
    }

    b.data |= this->wpawn_dir;
    b.data |= this->bpawn_dir<<1;
    b.data |= this->wcaptures<<2;
    b.data |= this->bcaptures<<5;
    b.data |= this->move_count << 10;

    b.hash ^= pawn_direction_h[0][wpawn_dir];
    b.hash ^= pawn_direction_h[1][bpawn_dir];
    b.hash ^= captures_h[0][wcaptures];
    b.hash ^= captures_h[1][bcaptures];

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

// Magic shit that i do not understand but is really cool or something
int PlayerCore::board_to_sq(int x){
    uint64_t y = x^(x-1);

    uint8_t z = (debruijn*y) >> 58;

    return lookup[z];
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
            bd.data ^= 1, bd.hash ^= pawn_direction_h[0][0]^pawn_direction_h[0][1];
        if(!(bd.data&1) && (bd.white&(15)))
            bd.data ^= 1, bd.hash ^= pawn_direction_h[0][0]^pawn_direction_h[0][1];
        
        if(!(bd.black&bmask)){
            bd.hash ^= pawn_direction_h[1][bd.data&2]^pawn_direction_h[1][default_pawnd^1];
            bd.data ^= (bd.data&2);
            bd.data |= ((this->default_pawnd^1)<<1);
        }

        // Apply move to bit board
        bd.black ^= ((move>>16)&bmask) << (move&63);
        bd.hash ^= pice_hash_code[((move&63)>>3)*2+1][board_to_sq(((move>>16)&bmask))];

        u64 nw = (move>>32)&bmask, cp = (nw|(nw<<16)|(nw<<32)|(nw<<48));
        bd.black |= nw<<(move&63);
        bd.hash ^= pice_hash_code[((move&63)>>3)*2+1][board_to_sq(nw)];

        if(bd.white & cp){
            bd.hash ^= captures_h[1][(bd.data>>5)&7] ^ captures_h[1][(bd.data>>5)&7+1];

            if(bd.white&bmask) bd.hash ^= pice_hash_code[0][board_to_sq(nw)];
            if((bd.white>>16)&bmask) bd.hash ^= pice_hash_code[2][board_to_sq(nw)];
            if((bd.white>>32)&bmask) bd.hash ^= pice_hash_code[4][board_to_sq(nw)];
            if((bd.white>>48)&bmask) bd.hash ^= pice_hash_code[6][board_to_sq(nw)];

            bd.data += 32;
            bd.white &= ~cp;
        }

        // Pawn shit
        if((bd.data&2) && (bd.black&(15<<12)))
            bd.data ^= 2, bd.hash ^= pawn_direction_h[1][0]^pawn_direction_h[1][1];
        if(!(bd.data&2) && (bd.black&(15)))
            bd.data ^= 2, bd.hash ^= pawn_direction_h[1][0]^pawn_direction_h[1][1];

        if(!(bd.white&bmask)){
            bd.hash ^= pawn_direction_h[0][bd.data&2]^pawn_direction_h[0][default_pawnd];

            bd.data ^= (bd.data&1);
            bd.data |= this->default_pawnd;
        }

    }else{ // The same shit but white, so it is a little better
        // Something i guess
        if((bd.data&2) && (bd.black&(15<<12)))
            bd.data ^= 2, bd.hash ^= pawn_direction_h[1][0]^pawn_direction_h[1][1];
        if(!(bd.data&2) && (bd.black&(15)))
            bd.data ^= 2, bd.hash ^= pawn_direction_h[1][0]^pawn_direction_h[1][1];
        
        if(!(bd.white&bmask)){
            bd.hash ^= pawn_direction_h[0][bd.data&2]^pawn_direction_h[0][default_pawnd];

            bd.data ^= (bd.data&1);
            bd.data |= this->default_pawnd;
        }

        // Apply move to bit board
        bd.white ^= ((move>>16)&bmask) << ((move&63));
        bd.hash ^= pice_hash_code[((move&63)>>3)*2][board_to_sq(((move>>16)&bmask))];

        u64 nw = (move>>32)&bmask, cp = (nw|(nw<<16)|(nw<<32)|(nw<<48));
        bd.white |= nw<<(move&63);
        bd.hash ^= pice_hash_code[((move&63)>>3)*2+1][board_to_sq(nw)];

        if(bd.black & cp){
            bd.hash ^= captures_h[0][(bd.data>>2)&7] ^ captures_h[0][(bd.data>>2)&7+1];

            if(bd.black&bmask) bd.hash ^= pice_hash_code[1][board_to_sq(nw)];
            if((bd.black>>16)&bmask) bd.hash ^= pice_hash_code[3][board_to_sq(nw)];
            if((bd.black>>32)&bmask) bd.hash ^= pice_hash_code[5][board_to_sq(nw)];
            if((bd.black>>48)&bmask) bd.hash ^= pice_hash_code[7][board_to_sq(nw)];

            bd.data += 4;      
            bd.black &= ~cp;
        }

        // Pawn shit
        if((bd.data&1) && (bd.white&(15<<12)))
            bd.data ^= 1, bd.hash ^= pawn_direction_h[0][0]^pawn_direction_h[0][1];
        if(!(bd.data&1) && (bd.white&(15)))
            bd.data ^= 1, bd.hash ^= pawn_direction_h[0][0]^pawn_direction_h[0][1];

        if(!(bd.black&bmask)){
            bd.hash ^= pawn_direction_h[1][bd.data&2]^pawn_direction_h[1][default_pawnd^1];
            bd.data ^= (bd.data&2);
            bd.data |= ((this->default_pawnd^1)<<1);
        }
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

    if(postion_history.count(bd.hash)){
        auto pos = postion_history[bd.hash];
        if(pos.second >= depth && pos.second + (bd.data>>10) <= 140) return pos.first;
    }

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
            //cout << "reached b win at white\n";
            return -inf;
        }
        
        if(moves.size() == 0){
            //cout << "No valid moves??\n";
            return -inf;
        }

        int v = -inf;
        
        for(u64 m: moves){
            board nx = this->aply_move(bd, m, 0);
            int nxv = this->alpha_beta(nx, depth-1, alpha, beta, 1);
            v = max(v, nxv);

            if((bd.data>>10) + depth < 140){
                if(!postion_history.count(nx.hash) || postion_history[nx.hash].second < depth)
                    postion_history[nx.hash] = {nxv, depth-1};
            }
        
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
            //cout << "reached b win at black\n";
            return -inf;
        }

        if(moves.size() == 0){
            //cout << "No valid moves??\n";
            return inf;
        }


        int v = inf;

        for(u64 m: moves){
            board nx = this->aply_move(bd, m, 1);
            int nxv = this->alpha_beta(nx, depth-1, alpha, beta, 0);
            v = min(v, nxv);

            if(!postion_history.count(nx.hash) || postion_history[nx.hash].second < depth)
                    postion_history[nx.hash] = {nxv, depth-1};

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

    int max_depth_reached = 0;

    for(int deep=1; deep<=140-move_count && (double)(clock() - start)/CLOCKS_PER_SEC < MAX_TIME; deep++){
        if(best >= inf)
            break;
        best = -inf-1;
        
        if(best_move != 0){
            int v = this->alpha_beta(this->aply_move(bd, best_move, 0), deep, -inf, inf, 1);

            if(v > best)
                best = v;
        }
        
        for(u64 m: moves){
            if(m==best_move)
                continue;

            int v = this->alpha_beta(this->aply_move(bd, m, 0), deep, -inf, inf, 1);

            if((double)(clock() - start)/CLOCKS_PER_SEC > MAX_TIME)
                break;

            if(v > best)
                best = v, best_move = m;
        }
        max_depth_reached ++;
    }

    cout << "Expected val: " << best << '\n';
    cout << "Cut offs: " << cut_offs << '\n';
    cout << "Searh depth: " << max_depth_reached << '\n';

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

    //postion_history.clear();
}