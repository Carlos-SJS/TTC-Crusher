#include <bits/stdc++.h>
using namespace std;

#define sq_to_bboard(x,y) (1<<((y<<2)+x))
#define valid(x, y) (x>=0&&y>=0&&x<4&&y<4)
#define in_range(x) (x>0&&x<3)


int main(){
    ofstream f("rook_rays.txt");

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            int r = 0;

            for(int k=1; k<3; k++) if(k != i) r |= sq_to_bboard(j, k);
            for(int k=1; k<3; k++) if(k != j) r |= sq_to_bboard(k, i);

            f << r << '\n';
        }
    }

    f.close();
}