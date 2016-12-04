#include "defs.h"

uint64_t SetMask[64];
uint64_t ClearMask[64];
uint64_t PieceKeys[13][64];
uint64_t SideKey;

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

void InitBitMasks() {
    for(int index = 0; index < 64; index++) {
        SetMask[index] = (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }
}

void InitHashKeys() {

    int index = 0;
    FILE *urand;
    urand = fopen("/dev/urandom", "rb");
    for(index = 0; index < 13; ++index) {
        fread(PieceKeys[index], sizeof(uint64_t), 64, urand);
    }
    fread(&SideKey, sizeof(uint64_t), 1, urand);
    fclose(urand);
}

void InitFilesRanksBrd() {

    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;

    for(index = 0; index < BRD_SQ_NUM; ++index) {
        FilesBrd[index] = OFFBOARD;
        RanksBrd[index] = OFFBOARD;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank) {
        for(file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file,rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
}


void AllInit(){
    InitBitMasks();
    InitHashKeys();
    InitFilesRanksBrd();
    InitSorcery();
}
