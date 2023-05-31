#include <bits/stdc++.h>
using namespace std;

#define sq_to_bboard(x,y) (1<<((y<<2)+x))

int get_bishop_moves(int y, int x, int blockers){
    vector<vector<bool>> bd(4, vector<bool>(4, 0));

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(blockers&sq_to_bboard(j, i)) bd[i][j] = 1;
        }
    }

    if(y > 1 && y < 3 && x > 1 && x < 3){
        cout << "--------------\n" << y << ", " << x << '\n';
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++) cout << (bd[i][j]?'X':(i==y&&j==x?'O':'.'));
            cout << '\n';
        }
    }

    int moves = 0;

    for(int i = x-1; i>=0; i--){
        moves |= sq_to_bboard(i, y);
        if(bd[y][i]) break;
    }
    for(int i = x+1; i<4; i++){
        moves |= sq_to_bboard(i, y);
        if(bd[y][i]) break;
    }

    for(int i = y-1; i>=0; i--){
        moves |= sq_to_bboard(x, i);
        if(bd[i][x]) break;
    }
    for(int i = y+1; i<4; i++){
        moves |= sq_to_bboard(x, i);
        if(bd[i][x]) break;
    }

    return moves;
}

int main(){
    ofstream f("rook_magics.txt");
    ofstream f2("rook_moves.txt");
    srand(time(NULL));

    int t = 0;

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            bool valid = 0;
            int magic;
            map<int, int> rook_table;
            while(!valid){
                t++;
                magic = rand()%(2<<16);
                //magic = 1;
                valid = 1;
                rook_table.clear();

                for(int k=0; k<(2<<5); k++){
                    int blockers = 0;

                    int c = k;
                    for(int l=1; l<3; l++){
                        if(l != i) blockers |= sq_to_bboard(j, l)*(c&1), c>>=1;
                    }

                    for(int l=1; l<3; l++){
                        if(l != j) blockers |= sq_to_bboard(l, i)*(c&1), c>>=1;
                    }

                    int h = (blockers*magic & ((1<<16)-1)) >> (16-4);
                    
                    if(rook_table.count(h)){
                        if(rook_table[h] != get_bishop_moves(i, j, blockers)){
                            valid = 0;
                            break;
                        }
                    }else rook_table[h] = get_bishop_moves(i, j, blockers);
                }
                
            }
            f << magic << '\n';
            for(auto p: rook_table){
                f2 << p.first << " " << p.second << '\n';
            }
            f2 << "-1\n";
        }
    }
    cout << t << '\n';
}