#include "defs.h"

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

uint64_t SetMask[64];
uint64_t ClearMask[64];
uint64_t PieceKeys[13][120];
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
		fread(PieceKeys[index], sizeof(uint64_t), 120, urand);
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


void InitSq120To64() {

	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	for(index = 0; index < BRD_SQ_NUM; ++index) {
		Sq120ToSq64[index] = 65;
	}

	for(index = 0; index < 64; ++index) {
		Sq64ToSq120[index] = 120;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			Sq64ToSq120[sq64] = sq;
			Sq120ToSq64[sq] = sq64;
			sq64++;
		}
	}
}

void AllInit(){

  InitSq120To64();
	InitBitMasks();
	InitHashKeys();
	InitFilesRanksBrd();

}
