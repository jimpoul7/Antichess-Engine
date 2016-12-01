#include "defs.h"

int Eval(S_BOARD *pos){


  return  50*(CountBits(CaptureSquares(pos,BLACK)) - CountBits(CaptureSquares(pos,WHITE))) + pos->material[WHITE] - pos->material[BLACK];

}

int Mobility(S_BOARD *pos, int side){

  uint64_t attacked;

  if(side == WHITE){
    // Black pieces attacked by WHITE
    attacked = CaptureSquares(pos,WHITE) & pos->occupied[BLACK];

    if(!CountBits(attacked)) return CountBits(MoveSquares(pos,WHITE));
    else return CountBits(attacked);
  }
  else{
    // White pieces attacked by BLACK
    attacked = CaptureSquares(pos,BLACK) & pos->occupied[WHITE];

    if(!CountBits(attacked)) return CountBits(MoveSquares(pos,BLACK));
    else return CountBits(attacked);
  }

}
