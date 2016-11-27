#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)
#define L1 ((uint64_t)1)

void AddMove(int move, int score, S_MOVELIST *list){
  list->moves[list->count].score = score;
  list->moves[list->count].move = move;
  list->count++;
}

void GenerateCaptures(const S_BOARD *pos, S_MOVELIST *list, uint64_t cap){
  int sq, from, move;
  uint64_t bbfrom, bbsq;
  if(pos->side == WHITE) {
    while(cap){
      sq = PopBit(&cap);
      bbsq;
      bbsq = L1 << sq;
      bbfrom = (BPawnEatSquares(bbsq) & pos->bitboards[wP] & (uint64_t)0x0000ffffffffffff) | (KingSquares(bbsq)  & pos->bitboards[wK]) | (KnightSquares(bbsq) & pos->bitboards[wN])
          | (RookSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[wR] | pos->bitboards[wQ])) | (BishopSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[wB] | pos->bitboards[wQ]));
      while(bbfrom){
        from = PopBit(&bbfrom);
        AddMove(MOVE(from, sq, pos->pieces[sq], EMPTY, 0), 0, list);
      }
      bbfrom = (BPawnEatSquares(bbsq) & pos->bitboards[wP] & (uint64_t)0x00ff000000000000);
      while(bbfrom){
        from = PopBit(&bbfrom);
        AddMove(MOVE(from, sq, pos->pieces[sq], wK, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], wR, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], wN, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], wB, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], wQ, 0), 0, list);
      }
    }
  }else{
    while(cap){
      sq = PopBit(&cap);
      bbsq;
      bbsq = L1 << sq;
      bbfrom = (WPawnEatSquares(bbsq) & pos->bitboards[bP] & (uint64_t)0xffffffffffff0000) | (KingSquares(bbsq)  & pos->bitboards[bK]) | (KnightSquares(bbsq) & pos->bitboards[bN])
          | (RookSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[bR] | pos->bitboards[bQ])) | (BishopSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[bB] | pos->bitboards[bQ])) ;
      while(bbfrom){
        from = PopBit(&bbfrom);
        AddMove(MOVE(from, sq, pos->pieces[sq], EMPTY, 0), 0, list);
      }
      bbfrom = (WPawnEatSquares(bbsq) & pos->bitboards[bP] & (uint64_t)0x000000000000ff00);
      while(bbfrom){
        from = PopBit(&bbfrom);
        AddMove(MOVE(from, sq, pos->pieces[sq], bK, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], bR, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], bN, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], bB, 0), 0, list);
        AddMove(MOVE(from, sq, pos->pieces[sq], bQ, 0), 0, list);
      }
    }
  }
}

void GenerateMoves(const S_BOARD *pos, S_MOVELIST *list){
  int to, from, move;
  uint64_t bb, bbto;
  if(pos->side == WHITE) {
    bbto = WPawnMoveSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH]) & (uint64_t)0x00ffffffffffffff;
    bb = WPawnMoveSquares(bbto) & (~pos->occupied[BOTH]) & (uint64_t)0x00000000ff000000;
    while(bbto){
      to = PopBit(&bbto);
      AddMove(MOVE(to-8, to, EMPTY, EMPTY, 0), 0, list);
    }
    while(bb){
      to = PopBit(&bb);
      AddMove(MOVE(to-16, to, EMPTY, EMPTY, MFLAGPS), 0, list);
    }

    bbto = WPawnMoveSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH]) & (uint64_t)0xff00000000000000;
    while(bbto){
      to = PopBit(&bbto);
      AddMove(MOVE(to-8, to, EMPTY, wK, 0), 0, list);
      AddMove(MOVE(to-8, to, EMPTY, wR, 0), 0, list);
      AddMove(MOVE(to-8, to, EMPTY, wK, 0), 0, list);
      AddMove(MOVE(to-8, to, EMPTY, wB, 0), 0, list);
      AddMove(MOVE(to-8, to, EMPTY, wQ, 0), 0, list);
    }

    bb = pos->bitboards[wK];
    while(bb){
      from = PopBit(&bb);
      bbto = KingSquares(L1 << from) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[wN];
    while(bb){
      from = PopBit(&bb);
      bbto = KnightSquares(L1 << from) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[wR];
    while(bb){
      from = PopBit(&bb);
      bbto = RookSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[wB];
    while(bb){
      from = PopBit(&bb);
      bbto = BishopSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[wQ];
    while(bb){
      from = PopBit(&bb);
      bbto = (BishopSquares(from, pos->occupied[BOTH]) | RookSquares(from, pos->occupied[BOTH])) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

  }else{
    bbto = BPawnMoveSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH]) & (uint64_t)0xffffffffffffff00;
    bb = BPawnMoveSquares(bbto) & (~pos->occupied[BOTH]) & (uint64_t)0x000000ff00000000;
    while(bbto){
      to = PopBit(&bbto);
      AddMove(MOVE(to+8, to, EMPTY, EMPTY, 0), 0, list);
    }
    while(bb){
      to = PopBit(&bb);
      AddMove(MOVE(to+16, to, EMPTY, EMPTY, MFLAGPS), 0, list);
    }

    bbto = BPawnMoveSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH]) & (uint64_t)0x00000000000000ff;
    while(bbto){
      to = PopBit(&bbto);
      AddMove(MOVE(to+8, to, EMPTY, bK, 0), 0, list);
      AddMove(MOVE(to+8, to, EMPTY, bR, 0), 0, list);
      AddMove(MOVE(to+8, to, EMPTY, bK, 0), 0, list);
      AddMove(MOVE(to+8, to, EMPTY, bB, 0), 0, list);
      AddMove(MOVE(to+8, to, EMPTY, bQ, 0), 0, list);
    }

    bb = pos->bitboards[bK];
    while(bb){
      from = PopBit(&bb);
      bbto = KingSquares(L1 << from) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[bN];
    while(bb){
      from = PopBit(&bb);
      bbto = KnightSquares(L1 << from) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[bR];
    while(bb){
      from = PopBit(&bb);
      bbto = RookSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[bB];
    while(bb){
      from = PopBit(&bb);
      bbto = BishopSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }

    bb = pos->bitboards[bQ];
    while(bb){
      from = PopBit(&bb);
      bbto = (BishopSquares(from, pos->occupied[BOTH]) | RookSquares(from, pos->occupied[BOTH])) & (~pos->occupied[BOTH]);
      while(bbto){
        to = PopBit(&bbto);
        AddMove(MOVE(from, to, EMPTY, EMPTY, 0), 0, list);
      }
    }
  }
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

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list){
  uint64_t bb;
  list->count = 0;
  if(pos->side == WHITE){
    if((pos->enPas != NO_SQ) && (bb = BPawnEatSquares(L1 << pos->enPas) & pos->bitboards[wP])){
      int from;
      while(bb){
        from = PopBit(&bb);
        AddMove(MOVE(from, pos->enPas, bP, EMPTY, MFLAGEP), 0, list);
      }
      bb = CaptureSquares(pos, WHITE) & pos->occupied[BLACK];
      if(bb) GenerateCaptures(pos, list, bb);
    }else{
      bb = CaptureSquares(pos, WHITE) & pos->occupied[BLACK];
      if(bb) GenerateCaptures(pos, list, bb);
      else GenerateMoves(pos, list);
    }
  }else{
    if((pos->enPas != NO_SQ) && (bb = WPawnEatSquares(L1 << pos->enPas) & pos->bitboards[bP])){
      int from;
      while(bb){
        from = PopBit(&bb);
        AddMove(MOVE(from, pos->enPas, wP, EMPTY, MFLAGEP), 0, list);
      }
      bb = CaptureSquares(pos, BLACK) & pos->occupied[WHITE];
      if(bb) GenerateCaptures(pos, list, bb);
    }else{
      bb = CaptureSquares(pos, BLACK) & pos->occupied[WHITE];
      if(bb) GenerateCaptures(pos, list, bb);
      else GenerateMoves(pos, list);
    }
  }
}
