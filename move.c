#include "defs.h"

char *PrSq(const int sq) {

	static char SqStr[3];

	int file = FilesBrd[sq];
	int rank = RanksBrd[sq];

	sprintf(SqStr, "%c%c", ('a'+file), ('1'+rank));

	return SqStr;

}

char *PrMove(int move) {

	static char MvStr[6];
  char pchar;

	int ff = FilesBrd[FROMSQ(move)];
	int rf = RanksBrd[FROMSQ(move)];
	int ft = FilesBrd[TOSQ(move)];
	int rt = RanksBrd[TOSQ(move)];

	int promoted = PROMOTED(move);

  if(promoted) {
    if( promoted == wQ || promoted ==bQ ){
      pchar = 'q';
    }
    else if( promoted == wK || promoted ==bK){
      pchar = 'k';
    }
    else if( promoted == wR || promoted ==bR){
      pchar = 'r';
    }
    else if( promoted == wN || promoted ==bN){
      pchar = 'n';
    }
    else if( promoted == wB || promoted ==bB){
      pchar = 'b';
    }
    sprintf(MvStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
	}
  else {
		sprintf(MvStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
	}

  return MvStr;

}

char *PrMove2(S_BOARD *pos, int move) {

	static char MvStr[12];
  char pchar;

	int ff = FilesBrd[FROMSQ(move)];
	int rf = RanksBrd[FROMSQ(move)];
	int ft = FilesBrd[TOSQ(move)];
	int rt = RanksBrd[TOSQ(move)];

	int promoted = PROMOTED(move);

  if(promoted) {
    if( promoted == wQ || promoted ==bQ ){
      pchar = 'Q';
    }
    else if( promoted == wK || promoted ==bK){
      pchar = 'K';
    }
    else if( promoted == wR || promoted ==bR){
      pchar = 'R';
    }
    else if( promoted == wN || promoted ==bN){
      pchar = 'N';
    }
    else if( promoted == wB || promoted ==bB){
      pchar = 'B';
    }
		if(CAPTURED(move)) sprintf(MvStr, "%cx%c%c=%c", ('a'+ff), ('a'+ft), ('1'+rt), pchar);
		else sprintf(MvStr, "%c%c=%c", ('a'+ft), ('1'+rt), pchar);

	}
  else {
		if(pos->pieces[FROMSQ(move)] == wP || pos->pieces[FROMSQ(move)] == bP){
			if(CAPTURED(move)) sprintf(MvStr, "%cx%c%c", ('a'+ff), ('a'+ft), ('1'+rt));
			else sprintf(MvStr, "%c%c", ('a'+ft), ('1'+rt));
		}else{
			uint64_t fromCands = 0;
			if(pos->pieces[FROMSQ(move)] == wK){
				fromCands = KingSquares(1UL << TOSQ(move)) & pos->bitboards[wK];
				MvStr[0] = 'K';
			}else if(pos->pieces[FROMSQ(move)] == bK){
				fromCands = KingSquares(1UL << TOSQ(move)) & pos->bitboards[bK];
				MvStr[0] = 'K';
			}else if(pos->pieces[FROMSQ(move)] == wN){
				fromCands = KnightSquares(1UL << TOSQ(move)) & pos->bitboards[wN];
				MvStr[0] = 'N';
			}else if(pos->pieces[FROMSQ(move)] == bN){
				fromCands = KnightSquares(1UL << TOSQ(move)) & pos->bitboards[bN];
				MvStr[0] = 'N';
			}
			else if(pos->pieces[FROMSQ(move)] == wR){
				fromCands = RookSquares(TOSQ(move), pos->occupied[BOTH]) & pos->bitboards[wR];
				MvStr[0] = 'R';
			}else if(pos->pieces[FROMSQ(move)] == bR){
				fromCands = RookSquares(TOSQ(move), pos->occupied[BOTH]) & pos->bitboards[bR];
				MvStr[0] = 'R';
			}
			else if(pos->pieces[FROMSQ(move)] == wB){
				fromCands = BishopSquares(TOSQ(move), pos->occupied[BOTH]) & pos->bitboards[wB];
				MvStr[0] = 'B';
			}else if(pos->pieces[FROMSQ(move)] == bB){
				fromCands = BishopSquares(TOSQ(move), pos->occupied[BOTH]) & pos->bitboards[bB];
				MvStr[0] = 'B';
			}
			else if(pos->pieces[FROMSQ(move)] == wQ){
				fromCands = (BishopSquares(TOSQ(move), pos->occupied[BOTH]) | RookSquares(TOSQ(move), pos->occupied[BOTH])) & pos->bitboards[wQ];
				MvStr[0] = 'Q';
			}else{
				fromCands = (BishopSquares(TOSQ(move), pos->occupied[BOTH]) | RookSquares(TOSQ(move), pos->occupied[BOTH])) & pos->bitboards[bQ];
				MvStr[0] = 'Q';
			}
			if(CountBits(fromCands) == 1){
				if(CAPTURED(move)) sprintf(MvStr+1, "x%c%c", ('a'+ft), ('1'+rt));
				else sprintf(MvStr+1, "%c%c", ('a'+ft), ('1'+rt));
			}else if(CountBits(fromCands & (0x0101010101010101UL << ff)) == 1){
				if(CAPTURED(move)) sprintf(MvStr+1, "%cx%c%c", ('a'+ff), ('a'+ft), ('1'+rt));
				else sprintf(MvStr+1, "%c%c%c", ('a'+ff), ('a'+ft), ('1'+rt));
			}else if(CountBits(fromCands & (0x00000000000000ffUL << rf*8)) == 1){
				if(CAPTURED(move)) sprintf(MvStr+1, "%cx%c%c", ('1'+rf), ('a'+ft), ('1'+rt));
				else sprintf(MvStr+1, "%c%c%c", ('1'+rf), ('a'+ft), ('1'+rt));
			}else{
				if(CAPTURED(move)) sprintf(MvStr+1, "%c%cx%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
				else sprintf(MvStr+1, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
			}
		}
	}

  return MvStr;

}

void PrintMoveList(const S_MOVELIST *list) {
	int index = 0;
	int score = 0;
	int move = 0;
	printf("MoveList:\n");

	for(index = 0; index < list->count; ++index) {

		move = list->moves[index].move;
		score = list->moves[index].score;

		printf("Move:%d > %s (score:%d)\n",index+1,PrMove(move),score);
	}
	printf("MoveList Total %d Moves:\n\n",list->count);
}

int ParseMove(char *ptrChar, S_BOARD *pos) {

	if(ptrChar[1] > '8' || ptrChar[1] < '1') return NOMOVE;
  if(ptrChar[3] > '8' || ptrChar[3] < '1') return NOMOVE;
  if(ptrChar[0] > 'h' || ptrChar[0] < 'a') return NOMOVE;
  if(ptrChar[2] > 'h' || ptrChar[2] < 'a') return NOMOVE;

  int from = FR2SQ(ptrChar[0] - 'a', ptrChar[1] - '1');
  int to = FR2SQ(ptrChar[2] - 'a', ptrChar[3] - '1');

	//assert(SqOnBoard(from) && SqOnBoard(to));

	S_MOVELIST list[1];
  GenerateAllMoves(pos,list);
  int MoveNum = 0;
	int Move = 0;
	int PromPce = EMPTY;

	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		Move = list->moves[MoveNum].move;
		if(FROMSQ(Move)==from && TOSQ(Move)==to) {
			PromPce = PROMOTED(Move);
			if(PromPce!=EMPTY) {
				if((PromPce == wR || PromPce == bR) && ptrChar[4]=='r') {
					return Move;
				} else if((PromPce == wB || PromPce == bB) && ptrChar[4]=='b') {
					return Move;
				} else if((PromPce == wQ || PromPce == bQ) && ptrChar[4]=='q') {
					return Move;
				} else if((PromPce == wN || PromPce == bN) && ptrChar[4]=='n') {
					return Move;
				} else if((PromPce == wK || PromPce == bK) && ptrChar[4]=='k') {
					return Move;
				}
				continue;
			}
			return Move;
		}
  }
  return NOMOVE;
}
