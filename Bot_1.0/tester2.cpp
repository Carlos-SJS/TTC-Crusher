#include<bits/stdc++.h>
using namespace std;

#include "core.hpp"
#include "util.hpp"

int main(){
    vector<vector<int>> b{
        { 0, 0, 0, 0},
        { 0, 4,-4, 0},
        { 0,-2, 0, 0},
        { 1, 0, 0, 3}
    };

    PlayerCore player;
    vector<vector<int>> b2 = player.getMove(b);

    cout << "DONE\n";
    cout << b2.size() << '\n';

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++) cout << b2[i][j] << ' ';
        cout << '\n';
    }
}