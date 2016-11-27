#include "defs.h"

#define HASH_PCE(pce,sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKeys[EMPTY][(pos->enPas)]))

static void ClearPiece(const int sq, S_BOARD *pos) {

  int pce = pos->pieces[sq];
	int col = PieceCol[pce];
	int index = 0;

  HASH_PCE(pce,sq);

	pos->pieces[sq] = EMPTY;
  pos->material[col] -= PieceVal[pce];

  CLRBIT(pos->bitboards[pce],sq);
  CLRBIT(pos->occupied[BOTH],sq);
  CLRBIT(pos->occupied[PieceCol[pce]],sq);

	pos->pceNum[pce]--;
}

static void AddPiece(const int sq, S_BOARD *pos, const int pce) {

  int col = PieceCol[pce];

	HASH_PCE(pce,sq);

	pos->pieces[sq] = pce;

  SETBIT(pos->bitboards[pce],sq);
  SETBIT(pos->occupied[BOTH],sq);
  SETBIT(pos->occupied[PieceCol[pce]],sq);

	pos->material[col] += PieceVal[pce];
	pos->pceNum[pce]++;

}

static void MovePiece(const int from, const int to, S_BOARD *pos) {

  int index = 0;
	int pce = pos->pieces[from];
	int col = PieceCol[pce];

	HASH_PCE(pce,from);
	pos->pieces[from] = EMPTY;

	HASH_PCE(pce,to);
	pos->pieces[to] = pce;

  SETBIT(pos->bitboards[pce],to);
  SETBIT(pos->occupied[BOTH],to);
  SETBIT(pos->occupied[col],to);

  CLRBIT(pos->bitboards[pce],from);
  CLRBIT(pos->occupied[BOTH],from);
  CLRBIT(pos->occupied[col],from);
}

void MakeMove(S_BOARD *pos, int move) {

	int from = FROMSQ(move);
  int to = TOSQ(move);
  int side = pos->side;

  pos->history[pos->hisPly].posKey = pos->posKey;

	if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to-8,pos);
        } else {
            ClearPiece(to+8,pos);
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
        ClearPiece(to, pos);
        pos->fiftyMove = 0;
  }

	pos->hisPly++;
	pos->ply++;

	if(pos->pieces[from] == wP || pos->pieces[from] == bP) {
        pos->fiftyMove = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->enPas=from+8;
                assert(RanksBrd[pos->enPas]==RANK_3);
            } else {
                pos->enPas=from-8;
                assert(RanksBrd[pos->enPas]==RANK_6);
            }
            HASH_EP;
        }
    }

	MovePiece(from, to, pos);

	int prPce = PROMOTED(move);
  if(prPce != EMPTY) {
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }

	pos->side ^= 1;
  HASH_SIDE;

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
            AddPiece(to-8, pos, bP);
        } else {
            AddPiece(to+8, pos , wP);
        }
    }

  MovePiece(to, from, pos);

	int captured = CAPTURED(move);

	if((captured != EMPTY) && !(MFLAGEP & move)) {
        AddPiece(to, pos, captured);
  }

  if(PROMOTED(move) != EMPTY)   {
        ClearPiece(from, pos);
        AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
    }

}
