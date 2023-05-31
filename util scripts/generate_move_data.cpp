#include <bits/stdc++.h>
using namespace std;

int main(){
    ifstream npmov("north_pawn.txt"), npcap("north_pawn_captures.txt"), spmov("south_pawn.txt"), spcap("south_pawn_captures.txt"), kmov("knight.txt"), rmag("rook_magics.txt"), rray("rook_rays.txt"), rmov("rook_moves.txt"), bmag("bishop_magics.txt"), bmov("bishop_moves.txt"), bray("bishop_rays.txt");
    ofstream mdata("move_data.hpp");

    int mv, id, ix;
    bool sp;

    mdata << "unsigned short int north_pawn_m[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            npmov >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int north_pawn_c[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            npcap >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int south_pawn_m[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            spmov >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int south_pawn_c[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            spcap >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int knight_m[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            kmov >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int rook_magics[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            rmag >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int rook_rays[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            rray >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int rook_m[16][16] = {\n";
    for(int i=0; i<16; i++){
        sp = ix = 0;
        mdata << "\t{";
        while(1){
            rmov >> id;
            if(id == -1) break;
            if(sp) mdata << ", ";
            else sp = 1;

            rmov >> mv;
            while(id > ix){
                mdata << "0, ";
                ix++;
            }
            mdata << mv;
            ix++;
        }
        while(ix < 16){
            if(sp) mdata << ", ";
            else sp = 1;
            mdata << '0';
            ix++;
        }
        mdata << "}";
        if(i != 15) mdata << ',';
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int bishop_magics[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            bmag >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

     mdata << "unsigned short int bishop_rays[] = {\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            bray >> mv;
            if(j == 0) mdata << '\t';
            mdata << mv;
            if(j!=3||i!=3) mdata << ',';
            if(j!=3) mdata << ' ';
        }
        mdata << '\n';
    }
    mdata << "};\n\n";

    mdata << "unsigned short int bishop_m[16][4] = {\n";
    for(int i=0; i<16; i++){
        sp = ix = 0;
        mdata << "\t{";
        while(1){
            bmov >> id;
            if(id == -1) break;
            if(sp) mdata << ", ";
            else sp = 1;

            bmov >> mv;
            while(id > ix){
                mdata << "0, ";
                ix++;
            }
            mdata << mv;
            ix++;
        }
        while(ix < 4){
            if(sp) mdata << ", ";
            else sp = 1;
            mdata << '0';
            ix++;
        }
        mdata << "}";
        if(i != 15) mdata << ',';
        mdata << '\n';
    }
    mdata << "};\n\n";
    

    npmov.close();
    spmov.close();
    npcap.close();
    spcap.close();
    kmov.close();
    rmov.close();
    bmov.close();
    rmag.close();
    bmag.close();
}