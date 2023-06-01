#pragma once

#include <vector>
#include <random>
#include <time.h>

#include "util.hpp"
using namespace std;

#define inf 100000000ll
#define bmask 65535ll

// This shit doesnt work >:v
#define horizontal(p) (p==15ll||p==(15ll<<4)||p==(15ll<<8)||p==(15ll<<12))
#define vertical(p) (p==vertical||p==(vertical<<1)||p==(vertical<<2)||p==(vertical<<3))
#define check_win(p) (p!=0 && (p==diagonal1 || p==diagonal2 || horizontal(p) || vertical(p)))

const u64 diagonal1 = 1ll|((1ll<<5))|(1ll<<10)|(1ll<<15);
const u64 diagonal2 = (1ll<<3)|(1ll<<6)|(1ll<<9)|(1ll<<12);
const u64 vertical = 1ll|(1ll<<4)|(1ll<<8)|(1ll<<12);

const double MAX_TIME = .8;

const int search_deepness = 4;

class PlayerCoreBase{
    private:
        // Fields
        int default_pawnd = 0;

        int wpawn_dir = 0; // 0 North, 1 South
        int bpawn_dir = 1;

        int wcaptures = 0;
        int bcaptures = 0;

        int cut_offs = 0;

        u64 move_count = 0;

        bool idle=1;

        clock_t start;
        clock_t current;

        u64 prev_board;

        // Methodes

        board vector_to_board(vector<vector<int>> &board); 
        vector<vector<int>> board_to_vector(board &b); 

        
        u64 find_move(board b);
        int alpha_beta(board &bd, int depth, int alpha, int beta, int player);
        int evaluate(board &bd);
        board aply_move(board bd, u64 &move, int player);

    public:

        PlayerCoreBase();
        void reset(int color);
        vector<vector<int>> getMove(vector<vector<int>>);

};