#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)

const int LoopSlidePce[8] = {
 wB, wR, wQ, 0, bB, bR, bQ, 0
};

const int LoopNonSlidePce[6] = {
 wN, wK, 0, bN, bK, 0
};

const int LoopSlideIndex[2] = { 0, 4 };
const int LoopNonSlideIndex[2] = { 0, 3 };

const int PceDir[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
};

const int NumDir[13] = {
 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
};

void addMove(S_MOVE move, S_MOVELIST *list){
  if(list->capture){
    if(CAPTURED(move.move)){
      list->moves[list->count++] = move;
    }
  }else{
    if(CAPTURED(move.move)){
      list->count = 1;
      list->capture = 1;
      list->moves[0] = move;
    }else{
      list->moves[list->count++] = move;
    }
  }
}



void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {

  list->count = 0;
  list->capture = 0;
  S_MOVE move;
  int i;
  int pce = EMPTY;
	int side = pos->side;
	int sq = 0;
  int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int index = 0;
	int pceIndex = 0;

  if(side == WHITE) {

		for(pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum) {
			sq = pos->pList[wP][pceNum];
			//ASSERT(SqOnBoard(sq));

			if(pos->pieces[sq + 10] == EMPTY) {
        if(sq > H6){
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], wN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], wB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], wR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], wQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], wK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq + 10, pos->pieces[sq + 10], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
				if(RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY) {
          move.move = MOVE(sq, sq + 20, pos->pieces[sq + 20], EMPTY, MFLAGPS);
          move.score = 0;
          addMove(move, list);
				}
			}

			if(!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK) {
        if(sq > H6){
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], wB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], wQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], wR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], wN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], wK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq + 9, pos->pieces[sq + 9], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
			}
			if(!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK) {
        if(sq > H6){
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], wB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], wQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], wR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], wN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], wK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq + 11, pos->pieces[sq + 11], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
			}

			if(pos->enPas != NO_SQ) {
				if(sq + 9 == pos->enPas) {
          move.move = MOVE(sq, sq + 9, bP, EMPTY, MFLAGEP);
          move.score = 0;
          addMove(move, list);
				}
				if(sq + 11 == pos->enPas) {
          move.move = MOVE(sq, sq + 11, bP, EMPTY, MFLAGEP);
          move.score = 0;
          addMove(move, list);
				}
			}
		}
  }
  else{

    for(pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum) {
			sq = pos->pList[bP][pceNum];
			//ASSERT(SqOnBoard(sq));

			if(pos->pieces[sq - 10] == EMPTY) {

        if(sq < A3){
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], bN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], bB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], bR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], bQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], bK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq - 10, pos->pieces[sq - 10], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
				if(RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY) {
          move.move = MOVE(sq, sq - 20, pos->pieces[sq - 20], EMPTY, MFLAGPS);
          move.score = 0;
          addMove(move, list);
				}
			}

      if(!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE) {
        if(sq < A3){
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], bB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], bQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], bR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], bN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], bK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq - 9, pos->pieces[sq - 9], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
			}
			if(!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE) {
        if(sq < A3){
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], bB, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], bQ, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], bR, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], bN, 0);
          move.score = 0;
          addMove(move, list);
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], bK, 0);
          move.score = 0;
          addMove(move, list);
        }else{
          move.move = MOVE(sq, sq - 11, pos->pieces[sq - 11], EMPTY, 0);
          move.score = 0;
          addMove(move, list);
        }
			}

			if(pos->enPas != NO_SQ) {
				if(sq - 9 == pos->enPas) {
          move.move = MOVE(sq, sq - 9, wP, EMPTY, MFLAGEP);
          move.score = 0;
          addMove(move, list);
				}
				if(sq - 11 == pos->enPas) {
          move.move = MOVE(sq, sq - 11, wP, EMPTY, MFLAGEP);
          move.score = 0;
          addMove(move, list);
				}
			}
		}
  }
  // if(pos->side == WHITE){
  //   for(i = 0; i < pos->pceNum[wN]; i++){
  //     int from = pos->pList[wN][i];
  //     if(!SQOFFBOARD(from + 21) && (PieceCol[pos->pieces[from + 21]] != WHITE)){
  //       move.move = MOVE(from, from + 21, pos->pieces[from + 21], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 19) && (PieceCol[pos->pieces[from + 19]] != WHITE)){
  //       move.move = MOVE(from, from + 19, pos->pieces[from + 19], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 8) && (PieceCol[pos->pieces[from + 8]] != WHITE)){
  //       move.move = MOVE(from, from + 8, pos->pieces[from + 8], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 12) && (PieceCol[pos->pieces[from + 12]] != WHITE)){
  //       move.move = MOVE(from, from + 12, pos->pieces[from + 12], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 21) && (PieceCol[pos->pieces[from - 21]] != WHITE)){
  //       move.move = MOVE(from, from - 21, pos->pieces[from - 21], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 19) && (PieceCol[pos->pieces[from - 19]] != WHITE)){
  //       move.move = MOVE(from, from - 19, pos->pieces[from - 19], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 8) && (PieceCol[pos->pieces[from - 8]] != WHITE)){
  //       move.move = MOVE(from, from - 8, pos->pieces[from - 8], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 12) && (PieceCol[pos->pieces[from - 12]] != WHITE)){
  //       move.move = MOVE(from, from - 12, pos->pieces[from - 12], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //   }
  //
  //   for(i = 0; i < pos->pceNum[wK]; i++){
  //     int from = pos->pList[wK][i];
  //     if(!SQOFFBOARD(from + 1) && (PieceCol[pos->pieces[from + 1]] != WHITE)){
  //       move.move = MOVE(from, from + 1, pos->pieces[from + 1], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 10) && (PieceCol[pos->pieces[from + 10]] != WHITE)){
  //       move.move = MOVE(from, from + 10, pos->pieces[from + 10], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 11) && (PieceCol[pos->pieces[from + 11]] != WHITE)){
  //       move.move = MOVE(from, from + 11, pos->pieces[from + 11], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from + 9) && (PieceCol[pos->pieces[from + 9]] != WHITE)){
  //       move.move = MOVE(from, from + 9, pos->pieces[from + 9], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 1) && (PieceCol[pos->pieces[from - 1]] != WHITE)){
  //       move.move = MOVE(from, from - 1, pos->pieces[from - 1], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 10) && (PieceCol[pos->pieces[from - 10]] != WHITE)){
  //       move.move = MOVE(from, from - 10, pos->pieces[from - 10], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 11) && (PieceCol[pos->pieces[from - 11]] != WHITE)){
  //       move.move = MOVE(from, from - 11, pos->pieces[from - 11], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //     if(!SQOFFBOARD(from - 9) && (PieceCol[pos->pieces[from - 9]] != WHITE)){
  //       move.move = MOVE(from, from - 9, pos->pieces[from - 9], EMPTY, 0);
  //       move.score = 0;
  //       addMove(move, list);
  //     }
  //   }
  // }

  /* Loop for slide pieces */
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while( pce != 0) {
		//ASSERT(PieceValid(pce));

		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pList[pce][pceNum];
			//ASSERT(SqOnBoard(sq));

		for(index = 0; index < NumDir[pce]; ++index) {
			dir = PceDir[pce][index];
			t_sq = sq + dir;

			while(!SQOFFBOARD(t_sq)) {
			     if(pos->pieces[t_sq] != EMPTY) {
			        if(PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
                move.move = MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0);
                move.score = 0;
                addMove(move, list);
					    }
              break;
            }
            move.move = MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0);
            move.score = 0;
            addMove(move, list);
					  t_sq += dir;
			}
		}
		}

		pce = LoopSlidePce[pceIndex++];
	}

	/* Loop for non slide */
	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];

	while( pce != 0) {
		//ASSERT(PieceValid(pce));

		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pList[pce][pceNum];
			//ASSERT(SqOnBoard(sq));

			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				if(SQOFFBOARD(t_sq)) {
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if(pos->pieces[t_sq] != EMPTY) {
					if( PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
            move.move = MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0);
            move.score = 0;
						addMove(move, list);
					}
					continue;
				}
        move.move = MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
			}
		}

		pce = LoopNonSlidePce[pceIndex++];
	}

    //ASSERT(MoveListOk(list,pos));

}
