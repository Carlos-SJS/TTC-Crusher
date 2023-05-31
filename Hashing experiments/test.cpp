#include <bits/stdc++.h>
using namespace std;

#define u64 unsigned long long

const u64 diagonal1 = 1ll|((1ll<<5))|(1ll<<10)|(1ll<<15);
const u64 diagonal2 = (1ll<<3)|(1ll<<6)|(1ll<<9)|(1ll<<12);
const u64 vertical = 1ll|(1ll<<4)|(1ll<<8)|(1ll<<12);

#define horizontal(p) (p==15||p==(15<<4)||p==(15<<8)||p==(15<<12))
#define vertical(p) (p==vertical||p==(vertical<<1)||p==(vertical<<2)||p==(vertical<<3))
#define check_win(p) (p!=0 && (p==diagonal1 || p==diagonal2 || horizontal(p) || vertical(p)))

void print_board(u64 b){
    vector<vector<char>> a(4, vector<char>(4, '.'));
    int x, y, ct = 0;
    x = 0, y = 0;
    cout << b << '\n';
    while(b>>ct){
        if((b>>ct)&1) a[y][x] = 'X';
        x++;
        if(x > 3) x = 0, y++;
        ct++;
        //b>>=1;
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            cout << a[i][j];
        cout << '\n';
    }
}


int main(){
    u64 p = (1<<3)|(1<<6)|(1<<9)|(1<<12);

    print_board(p);
    cout << check_win(p) << '\n';

}