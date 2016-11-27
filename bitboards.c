#include "defs.h"

int PopBit(uint64_t *bb) {
  int index;
  index = __builtin_ctzll(*bb);
  *bb &= *bb - 1;
  //*bb ^= (((uint64_t)1) << index);
  return index;
}

int CountBits(uint64_t b) {
  return __builtin_popcountl(b);
}

uint64_t KingSquares(uint64_t b){
	return (((b << 1) | (b << 9) | (b >> 7)) & 0xfefefefefefefefe) | (((b >> 1) | (b >> 9) | (b << 7)) & 0x7f7f7f7f7f7f7f7f) | (b << 8) | (b >> 8);
}

uint64_t KnightSquares(uint64_t b){
	return (((b << 17) | (b >> 15)) & 0xfefefefefefefefe) | (((b >> 17) | (b << 15)) & 0x7f7f7f7f7f7f7f7f) | (((b << 10) | (b >> 6)) & 0xfcfcfcfcfcfcfcfc) | (((b >> 10) | (b << 6)) & 0x3f3f3f3f3f3f3f3f);
}

uint64_t WPawnEatSquares(uint64_t b){
	return ((b << 9) & 0xfefefefefefefefe) | ((b << 7) & 0x7f7f7f7f7f7f7f7f);
}

uint64_t BPawnEatSquares(uint64_t b){
	return ((b >> 9) & 0x7f7f7f7f7f7f7f7f) | ((b >> 7) & 0xfefefefefefefefe);
}

uint64_t WPawnMoveSquares(uint64_t b){
	return (b << 8);
}

uint64_t BPawnMoveSquares(uint64_t b){
	return (b >> 8);
}

uint64_t WPawnJumpSquares(uint64_t b){
	return (b << 16) & 0xff000000;
}

uint64_t BPawnJumpSquares(uint64_t b){
	return (b >> 16) & 0xff00000000;
}



void PrintBitBoard(uint64_t bb) {

	uint64_t shiftMe = 1ULL;

	int rank = 0;
	int file = 0;
	int sq = 0;

	printf("\n");
	for(rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);

			if((shiftMe << sq) & bb)
				printf("X");
			else
				printf("-");

		}
		printf("\n");
	}
    printf("\n\n");
}
