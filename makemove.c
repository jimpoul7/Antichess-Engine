#include "defs.h"

#define HASH_PCE(pce,sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKeys[EMPTY][(pos->enPas)]))

static void ClearPiece(const int sq, S_BOARD *pos) {

	//assert(SqOnBoard(sq));
	//assert(CheckBoard(pos));

  int pce = pos->pieces[sq];

  //assert(PieceValid(pce));

	int col = PieceCol[pce];
	int index = 0;
	int t_pceNum = -1;

	//assert(SideValid(col));

  HASH_PCE(pce,sq);

	pos->pieces[sq] = EMPTY;
  pos->material[col] -= PieceVal[pce];

	if( pce == wP || pce == bP ){
		CLRBIT(pos->pawns[col],SQ64(sq));
		CLRBIT(pos->pawns[BOTH],SQ64(sq));
	}

	for(index = 0; index < pos->pceNum[pce]; ++index) {
		if(pos->pList[pce][index] == sq) {
			t_pceNum = index;
			break;
		}
	}

	//assert(t_pceNum != -1);
	//assert(t_pceNum>=0&&t_pceNum<10);

	pos->pceNum[pce]--;

	pos->pList[pce][t_pceNum] = pos->pList[pce][pos->pceNum[pce]];

}

static void AddPiece(const int sq, S_BOARD *pos, const int pce) {

  //assert(PieceValid(pce));
  //assert(SqOnBoard(sq));

	int col = PieceCol[pce];

	//assert(SideValid(col));

  HASH_PCE(pce,sq);

	pos->pieces[sq] = pce;

  if(pce == wP || pce == bP){
		SETBIT(pos->pawns[col],SQ64(sq));
		SETBIT(pos->pawns[BOTH],SQ64(sq));
	}

	pos->material[col] += PieceVal[pce];
	pos->pList[pce][pos->pceNum[pce]++] = sq;

}

static void MovePiece(const int from, const int to, S_BOARD *pos) {

  //assert(SqOnBoard(from));
  //assert(SqOnBoard(to));

	int index = 0;
	int pce = pos->pieces[from];
	int col = PieceCol[pce];
	//ASSERT(SideValid(col));
  //ASSERT(PieceValid(pce));


	HASH_PCE(pce,from);
	pos->pieces[from] = EMPTY;

	HASH_PCE(pce,to);
	pos->pieces[to] = pce;

	if(pce == wP || pce == bP) {
		CLRBIT(pos->pawns[col],SQ64(from));
		CLRBIT(pos->pawns[BOTH],SQ64(from));
		SETBIT(pos->pawns[col],SQ64(to));
		SETBIT(pos->pawns[BOTH],SQ64(to));
	}

	for(index = 0; index < pos->pceNum[pce]; ++index) {
		if(pos->pList[pce][index] == from) {
			pos->pList[pce][index] = to;
			break;
		}
	}
}

void MakeMove(S_BOARD *pos, int move) {

	//ASSERT(CheckBoard(pos));

	int from = FROMSQ(move);
  int to = TOSQ(move);
  int side = pos->side;

	//assert(SqOnBoard(from));
  //assert(SqOnBoard(to));
  //ASSERT(SideValid(side));
  //ASSERT(PieceValid(pos->pieces[from]));
	//assert(pos->hisPly >= 0 && pos->hisPly < MAXGAMEMOVES);
	//assert(pos->ply >= 0 && pos->ply < MAXDEPTH);

	pos->history[pos->hisPly].posKey = pos->posKey;

	if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to-10,pos);
        } else {
            ClearPiece(to+10,pos);
        }
    }

	if(pos->enPas != NO_SQ) HASH_EP;

	pos->history[pos->hisPly].move = move;
  pos->history[pos->hisPly].fiftyMove = pos->fiftyMove;
  pos->history[pos->hisPly].enPas = pos->enPas;

  pos->enPas = NO_SQ;

	int captured = CAPTURED(move);
  pos->fiftyMove++;

	if((captured != EMPTY) && (!(move & MFLAGEP))) {
        //ASSERT(PieceValid(captured));
        ClearPiece(to, pos);
        pos->fiftyMove = 0;
  }

	pos->hisPly++;
	pos->ply++;

	//assert(pos->hisPly >= 0 && pos->hisPly < MAXGAMEMOVES);
	//assert(pos->ply >= 0 && pos->ply < MAXDEPTH);

	if(pos->pieces[from] == wP || pos->pieces[from] == bP) {
        pos->fiftyMove = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->enPas=from+10;
                assert(RanksBrd[pos->enPas]==RANK_3);
            } else {
                pos->enPas=from-10;
                assert(RanksBrd[pos->enPas]==RANK_6);
            }
            HASH_EP;
        }
    }

	MovePiece(from, to, pos);

	int prPce = PROMOTED(move);
  if(prPce != EMPTY) {
        //ASSERT(PieceValid(prPce) && !PiecePawn[prPce]);
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }

	pos->side ^= 1;
  HASH_SIDE;

  //ASSERT(CheckBoard(pos));

}

void UndoMove(S_BOARD *pos) {

  pos->hisPly--;
  pos->ply--;
  pos->side ^= 1;
  HASH_SIDE;

  int move = pos->history[pos->hisPly].move;
  int from = FROMSQ(move);
  int to = TOSQ(move);

	if(pos->enPas != NO_SQ) HASH_EP;

  pos->fiftyMove = pos->history[pos->hisPly].fiftyMove;
  pos->enPas = pos->history[pos->hisPly].enPas;

  if(pos->enPas != NO_SQ) HASH_EP;

	if(MFLAGEP & move) {
        if(pos->side == WHITE) {
            AddPiece(to-10, pos, bP);
        } else {
            AddPiece(to+10, pos , wP);
        }
    }

  MovePiece(to, from, pos);

	int captured = CAPTURED(move);

	if((captured != EMPTY) && !(MFLAGEP & move)) {
        //ASSERT(PieceValid(captured));
        AddPiece(to, pos, captured);
  }

  if(PROMOTED(move) != EMPTY)   {
        //ASSERT(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
        ClearPiece(from, pos);
        AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
    }

}
