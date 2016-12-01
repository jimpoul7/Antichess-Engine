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

uint64_t CaptureSquares(const S_BOARD *pos, int side){
  uint64_t cap;
  uint64_t sbb;
  int sq;
  if(side == WHITE){
    cap = WPawnEatSquares(pos->bitboards[wP]) | KingSquares(pos->bitboards[wK]) | KnightSquares(pos->bitboards[wN]);

    sbb = pos->bitboards[wR];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= RookSquares(sq, pos->occupied[BOTH]);
    }

    sbb = pos->bitboards[wB];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= BishopSquares(sq, pos->occupied[BOTH]);
    }

    sbb = pos->bitboards[wQ];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= BishopSquares(sq, pos->occupied[BOTH]) | RookSquares(sq, pos->occupied[BOTH]);
    }

  }else{
    cap = BPawnEatSquares(pos->bitboards[bP]) | KingSquares(pos->bitboards[bK]) | KnightSquares(pos->bitboards[bN]);

    sbb = pos->bitboards[bR];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= RookSquares(sq, pos->occupied[BOTH]);
    }

    sbb = pos->bitboards[bB];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= BishopSquares(sq, pos->occupied[BOTH]);
    }

    sbb = pos->bitboards[bQ];
    while(sbb){
      sq = PopBit(&sbb);
      cap |= BishopSquares(sq, pos->occupied[BOTH]) | RookSquares(sq, pos->occupied[BOTH]);
    }
  }
  return cap;
}

uint64_t MoveSquares(const S_BOARD *pos, int side){

  uint64_t move_sqs;
  uint64_t bb;
  int sq;
  if(side == WHITE){
    move_sqs = (WPawnMoveSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH])) | (WPawnJumpSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH]))
                  | (KingSquares(pos->bitboards[wK]) & (~pos->occupied[BOTH])) | (KnightSquares(pos->bitboards[wN]) & (~pos->occupied[BOTH]));

    bb = pos->bitboards[wB];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= BishopSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }

    bb = pos->bitboards[wR];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= RookSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }

    bb = pos->bitboards[wQ];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= BishopSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]) | RookSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }

  }else{
    move_sqs = (BPawnMoveSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH])) | (BPawnJumpSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH]))
                  | (KingSquares(pos->bitboards[bK]) & (~pos->occupied[BOTH])) | (KnightSquares(pos->bitboards[bN]) & (~pos->occupied[BOTH]));

    bb = pos->bitboards[bB];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= BishopSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }

    bb = pos->bitboards[bR];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= RookSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }

    bb = pos->bitboards[bQ];
    while(bb){
      sq = PopBit(&bb);
      move_sqs |= BishopSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]) | RookSquares(sq, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
    }
  }
  return move_sqs;
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
