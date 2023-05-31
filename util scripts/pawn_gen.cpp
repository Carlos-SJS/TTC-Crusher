#include <bits/stdc++.h>
using namespace std;

#define valid(x, y) (x>=0&&y>=0&&x<4&&y<4)
#define sq_to_bboard(x,y) (1<<((y<<2)+x))

int main(){
    ofstream f1("north_pawn.txt"), f2("south_pawn.txt");
    ofstream fc1("north_pawn_captures.txt"), fc2("south_pawn_captures.txt");

    for(int i=0;  i<4; i++)
        for(int j=0; j<4; j++){
            int mv1 = 0, mv2 = 0, cp1 = 0, cp2 = 0;
            if(valid(j-1,i-1)) cp1 |= sq_to_bboard(j-1,i-1);
            if(valid(j,i-1)) mv1 |= sq_to_bboard(j,i-1);
            if(valid(j+1,i-1)) cp1 |= sq_to_bboard(j+1,i-1);

            if(valid(j-1,i+1)) cp2 |= sq_to_bboard(j-1,i+1);
            if(valid(j,i+1)) mv2 |= sq_to_bboard(j,i+1);
            if(valid(j+1,i+1)) cp2 |= sq_to_bboard(j+1,i+1);

            f1 << mv1 << '\n';
            fc1 << cp1 << '\n';
            f2 << mv2 << '\n';            
            fc2 << cp2 << '\n';            
        }
    
    f1.close();
    f2.close();
    fc1.close();
    fc2.close();
}