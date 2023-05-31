#include<bits/stdc++.h>
using namespace std;

int main(){
    ofstream f("hashing_rands.txt");

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dis(0, 1e18);

    dis(gen);

    for(int p=0; p<2; p++){
        f << "Values for player " << p << "\n";
        for(int i=1; i<=4; i++){    
            f << "Pice " << i << ": [";

            for(int i=0; i<16; i++){
                if(i > 0) f << ", ";
                f << dis(gen);
            }

            f << "]\n";
        }

        f << "Captures " << ": [";
        for(int i=0; i<=5; i++){
            if(i > 0) f << ", ";
            f << dis(gen);
        }
        f << "]\n";

        f << "North pawn: " << dis(gen) << '\n';
        f << "South pawn: " << dis(gen) << "\n\n";
    }
}