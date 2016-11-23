#include "defs.h"

int Eval(S_BOARD *pos){

	S_MOVELIST list;
  int score = 1;

  GenerateAllMoves(pos,&list);

  if(pos->side == WHITE){
    score = list.count;
  }
  else{
    score = -list.count;
  }

  return score;

}
