#pragma once

#include <vector>
#include <random>
#include <time.h>

#include "util.hpp"
using namespace std;

class PlayerCore{
    private:
        // Fields
        int default_pawnd = 0;

        int wpawn_dir = 0; // 0 North, 1 South
        int bpawn_dir = 1;

        int wcaptures = 0;
        int bcaptures = 0;

        u64 move_count = 0;

        // Methodes

        board vector_to_board(vector<vector<int>> &board); 
        vector<vector<int>> board_to_vector(board &b); 

    public:

        PlayerCore();
        void reset(int color);
        vector<vector<int>> getMove(vector<vector<int>>);

};