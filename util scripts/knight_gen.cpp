#include <bits/stdc++.h>
using namespace std;

#define valid(x, y) (x>=0&&y>=0&&x<4&&y<4)
#define sq_to_bboard(x,y) (1<<((y<<2)+x))

int main(){
    ofstream f1("knight.txt");

    for(int i=0;  i<4; i++)
        for(int j=0; j<4; j++){
            int m = 0;
            if(valid(j-1, i-2)) m |= sq_to_bboard(j-1, i-2);
            if(valid(j-1, i+2)) m |= sq_to_bboard(j-1, i+2);
            if(valid(j+1, i-2)) m |= sq_to_bboard(j+1, i-2);
            if(valid(j+1, i+2)) m |= sq_to_bboard(j+1, i+2);

            if(valid(j-2, i-1)) m |= sq_to_bboard(j-2, i-1);
            if(valid(j-2, i+1)) m |= sq_to_bboard(j-2, i+1);
            if(valid(j+2, i-1)) m |= sq_to_bboard(j+2, i-1);
            if(valid(j+2, i+1)) m |= sq_to_bboard(j+2, i+1);

            f1 << m << '\n';    
        }
    
    f1.close();
}