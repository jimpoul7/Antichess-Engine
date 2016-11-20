#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)

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

  if(pos->side == WHITE){
    for(i = 0; i < pos->pceNum[wN]; i++){
      int from = pos->pList[wN][i];
      if(!SQOFFBOARD(from + 21) && (PieceCol[pos->pieces[from + 21]] != WHITE)){
        move.move = MOVE(from, from + 21, pos->pieces[from + 21], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 19) && (PieceCol[pos->pieces[from + 19]] != WHITE)){
        move.move = MOVE(from, from + 19, pos->pieces[from + 19], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 8) && (PieceCol[pos->pieces[from + 8]] != WHITE)){
        move.move = MOVE(from, from + 8, pos->pieces[from + 8], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 12) && (PieceCol[pos->pieces[from + 12]] != WHITE)){
        move.move = MOVE(from, from + 12, pos->pieces[from + 12], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 21) && (PieceCol[pos->pieces[from - 21]] != WHITE)){
        move.move = MOVE(from, from - 21, pos->pieces[from - 21], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 19) && (PieceCol[pos->pieces[from - 19]] != WHITE)){
        move.move = MOVE(from, from - 19, pos->pieces[from - 19], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 8) && (PieceCol[pos->pieces[from - 8]] != WHITE)){
        move.move = MOVE(from, from - 8, pos->pieces[from - 8], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 12) && (PieceCol[pos->pieces[from - 12]] != WHITE)){
        move.move = MOVE(from, from - 12, pos->pieces[from - 12], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
    }

    for(i = 0; i < pos->pceNum[wK]; i++){
      int from = pos->pList[wK][i];
      if(!SQOFFBOARD(from + 1) && (PieceCol[pos->pieces[from + 1]] != WHITE)){
        move.move = MOVE(from, from + 1, pos->pieces[from + 1], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 10) && (PieceCol[pos->pieces[from + 10]] != WHITE)){
        move.move = MOVE(from, from + 10, pos->pieces[from + 10], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 11) && (PieceCol[pos->pieces[from + 11]] != WHITE)){
        move.move = MOVE(from, from + 11, pos->pieces[from + 11], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from + 9) && (PieceCol[pos->pieces[from + 9]] != WHITE)){
        move.move = MOVE(from, from + 9, pos->pieces[from + 9], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 1) && (PieceCol[pos->pieces[from - 1]] != WHITE)){
        move.move = MOVE(from, from - 1, pos->pieces[from - 1], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 10) && (PieceCol[pos->pieces[from - 10]] != WHITE)){
        move.move = MOVE(from, from - 10, pos->pieces[from - 10], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 11) && (PieceCol[pos->pieces[from - 11]] != WHITE)){
        move.move = MOVE(from, from - 11, pos->pieces[from - 11], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
      if(!SQOFFBOARD(from - 9) && (PieceCol[pos->pieces[from - 9]] != WHITE)){
        move.move = MOVE(from, from - 9, pos->pieces[from - 9], EMPTY, 0);
        move.score = 0;
        addMove(move, list);
      }
    }

    



  }

}
