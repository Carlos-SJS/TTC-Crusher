#include <bits/stdc++.h>
using namespace std;

#define sq_to_bboard(x,y) (1<<((y<<2)+x))
#define valid(x, y) (x>=0&&y>=0&&x<4&&y<4)
#define in_range(x) (x>0&&x<3)

int get_bishop_moves(int y, int x, int blockers){
    vector<vector<bool>> bd(4, vector<bool>(4, 0));

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(blockers&sq_to_bboard(j, i)) bd[i][j] = 1;
        }
    }

    /*cout << "--------------\n" << y << ", " << x << '\n';
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++) cout << (bd[i][j]?'X':(i==y&&j==x?'O':'.'));
        cout << '\n';
    }*/

    int moves = 0;

    int i = 0;
    while(valid(x-i, y-i)){
        moves |= sq_to_bboard(x-i, y-i);
        if(bd[y-i][x-i]) break;
        i++;
    }
    i=0;
    while(valid(x-i, y+i)){
        moves |= sq_to_bboard(x-i, y+i);
        if(bd[y+i][x-i]) break;
        i++;
    }
    i = 0;
    while(valid(x+i, y-i)){
        moves |= sq_to_bboard(x+i, y-i);
        if(bd[y-i][x+i]) break;
        i++;
    }
    i=0;
    while(valid(x+i, y+i)){
        moves |= sq_to_bboard(x+i, y+i);
        if(bd[y+i][x+i]) break;
        i++;
    }
    

    return moves;
}

int main(){
    ofstream f("bishop_magics.txt");
    ofstream f2("bishop_moves.txt");
    srand(time(NULL));

    int t = 0;

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            bool valid = 0;
            int magic;
            map<int, int> bishop_table;
            while(!valid){
                t++;
                magic = rand()%(2<<16);
                //magic = 1;
                valid = 1;
                bishop_table.clear();

                for(int k=0; k<(2<<5); k++){
                    int blockers = 0;

                    int c = k;
                    for(int k = -4; k<4; k++){
                        if(k != 0 && in_range(i+k) && in_range(j+k)) blockers |= sq_to_bboard(j+k, i+k)*(c&1), c>>=1;
                        if(k != 0 && in_range(i+k) && in_range(j-k)) blockers |= sq_to_bboard(j-k, i+k)*(c&1), c>>=1;
                    }

                    int h = (blockers*magic & ((1<<16)-1)) >> (16-2);
                    
                    if(bishop_table.count(h)){
                        if(bishop_table[h] != get_bishop_moves(i, j, blockers)){
                            valid = 0;
                            break;
                        }
                    }else bishop_table[h] = get_bishop_moves(i, j, blockers);
                }
                
            }
            f << magic << '\n';
            for(auto p: bishop_table){
                f2 << p.first << " " << p.second << '\n';
            }
            f2 << "-1\n";
        }
    }
    cout << t << '\n';
}