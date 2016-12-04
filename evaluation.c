#include "defs.h"

int Eval(S_BOARD *pos){

    int material_difference;
    int attacked_squares;
    int mobility;

    material_difference = pos->material[WHITE] - pos->material[BLACK];
    attacked_squares = CountBits(CaptureSquares(pos,BLACK)) - CountBits(CaptureSquares(pos,WHITE));
    mobility = Mobility(pos,WHITE) -  Mobility(pos,BLACK);

    return  15*attacked_squares + material_difference + 15*mobility + 1000*(pos->bitboards[bP]==pos->occupied[BLACK]) -1000*(pos->bitboards[wP]==pos->occupied[WHITE]);

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
