#include <bits/stdc++.h>
using namespace std;

#define sq_to_bboard(x,y) (1<<((y<<2)+x))
#define valid(x, y) (x>=0&&y>=0&&x<4&&y<4)
#define in_range(x) (x>0&&x<3)

void print_board(unsigned long long b){
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

int main(){
    ofstream f("bishop_rays.txt");

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            int r = 0;

            for(int k=-3; k<4; k++){
                if(k!=0 && in_range(i+k) && in_range(j+k)) r |= sq_to_bboard(j+k, i+k);
                if(k!=0 && in_range(i+k) && in_range(j-k)) r |= sq_to_bboard(j-k, i+k);
            }
            print_board(r);
            f << r << '\n';
        }
    }

    f.close();
}