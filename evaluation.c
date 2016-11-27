#include "defs.h"

int Eval(S_BOARD *pos){

  return  CountBits(CaptureSquares(pos,BLACK)) - CountBits(CaptureSquares(pos,WHITE));

}
