#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)
#define L1 ((uint64_t)1)

void AddMove(const S_BOARD *pos, int move, int score, S_MOVELIST *list){

    int index;
    index = pos->posKey % pos->HashTable->numofEntries;

    if(pos->HashTable->pTable[index].posKey == pos->posKey && pos->HashTable->pTable[index].move == move){
        score = 100000;
    }
    else if(CAPTURED(move)){
        if(pos->captureKillers[0][pos->ply] == move) score = 30000;
        else if(pos->captureKillers[1][pos->ply] == move) score = 20000;
    }
    else{
        if(pos->quietKillers[0][pos->ply] == move) score = 30000;
        else if(pos->quietKillers[1][pos->ply] == move) score = 20000;
    }
    list->moves[list->count].score = score;
    list->moves[list->count].move = move;
    list->count++;
}

void GenerateCaptures(const S_BOARD *pos, S_MOVELIST *list, uint64_t cap){
    int sq, from, move, atsq_value;
    uint64_t bbfrom, bbsq, atsq;
    if(pos->side == WHITE) {
        atsq = CaptureSquares(pos,BLACK);
        while(cap){
            sq = PopBit(&cap);
            bbsq;
            bbsq = L1 << sq;
            bbfrom = (BPawnEatSquares(bbsq) & pos->bitboards[wP] & (uint64_t)0x0000ffffffffffff) | (KingSquares(bbsq)  & pos->bitboards[wK]) | (KnightSquares(bbsq) & pos->bitboards[wN])
            | (RookSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[wR] | pos->bitboards[wQ])) | (BishopSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[wB] | pos->bitboards[wQ]));
            while(bbfrom){
                from = PopBit(&bbfrom);
                if((L1 << sq) & atsq ) atsq_value = -PieceVal[pos->pieces[from]];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], EMPTY, 0), PieceVal[pos->pieces[sq]] + atsq_value, list);
            }
            bbfrom = (BPawnEatSquares(bbsq) & pos->bitboards[wP] & (uint64_t)0x00ff000000000000);
            while(bbfrom){
                from = PopBit(&bbfrom);
                if((L1 << sq) & atsq ) atsq_value = 0;
                else atsq_value = 0xffffffff;
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], wK, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], wR, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], wN, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], wB, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], wQ, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
            }
        }
    }else{
        atsq = CaptureSquares(pos,WHITE);
        while(cap){
            sq = PopBit(&cap);
            bbsq;
            bbsq = L1 << sq;
            bbfrom = (WPawnEatSquares(bbsq) & pos->bitboards[bP] & (uint64_t)0xffffffffffff0000) | (KingSquares(bbsq)  & pos->bitboards[bK]) | (KnightSquares(bbsq) & pos->bitboards[bN])
            | (RookSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[bR] | pos->bitboards[bQ])) | (BishopSquares(sq, pos->occupied[BOTH]) & (pos->bitboards[bB] | pos->bitboards[bQ])) ;
            while(bbfrom){
                from = PopBit(&bbfrom);
                if((L1 << sq) & atsq ) atsq_value = -PieceVal[pos->pieces[from]];
                else atsq_value = 0;
                if(pos->captureKillers[0][pos->ply] == MOVE(from, sq, pos->pieces[sq], EMPTY, 0)) {
                    AddMove(pos, MOVE(from, sq, pos->pieces[sq], EMPTY, 0), 30000, list);
                } else if(pos->captureKillers[1][pos->ply] == MOVE(from, sq, pos->pieces[sq], EMPTY, 0)) {
                    AddMove(pos, MOVE(from, sq, pos->pieces[sq], EMPTY, 0), 20000, list);
                } else {
                    AddMove(pos, MOVE(from, sq, pos->pieces[sq], EMPTY, 0), PieceVal[pos->pieces[sq]]+ atsq_value, list);
                }
            }
            bbfrom = (WPawnEatSquares(bbsq) & pos->bitboards[bP] & (uint64_t)0x000000000000ff00);
            while(bbfrom){
                from = PopBit(&bbfrom);
                if((L1 << sq) & atsq ) atsq_value = 0;
                else atsq_value = 0xffffffff;
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], bK, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], bR, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], bN, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], bB, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
                AddMove(pos, MOVE(from, sq, pos->pieces[sq], bQ, 0), PieceVal[pos->pieces[sq]]+(atsq_value & PieceVal[wK])-PieceVal[wP], list);
            }
        }
    }
}

void GenerateMoves(const S_BOARD *pos, S_MOVELIST *list){
    int to, from, move, atsq_value;
    uint64_t bb, bbto, atsq;
    if(pos->side == WHITE) {
        atsq = CaptureSquares(pos,BLACK);
        bbto = WPawnMoveSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH]) & (uint64_t)0x00ffffffffffffff;
        bb = WPawnMoveSquares(bbto) & (~pos->occupied[BOTH]) & (uint64_t)0x00000000ff000000;
        while(bbto){
            to = PopBit(&bbto);
            if((L1 << to) & atsq ) atsq_value = -PieceVal[wP];
            else atsq_value = 0;
            AddMove(pos, MOVE(to-8, to, EMPTY, EMPTY, 0), atsq_value, list);
        }
        while(bb){
            to = PopBit(&bb);
            if((L1 << to) & atsq ) atsq_value = -PieceVal[wP];
            else atsq_value = 0;
            AddMove(pos, MOVE(to-16, to, EMPTY, EMPTY, MFLAGPS), atsq_value, list);
        }

        bbto = WPawnMoveSquares(pos->bitboards[wP]) & (~pos->occupied[BOTH]) & (uint64_t)0xff00000000000000;
        while(bbto){
            to = PopBit(&bbto);
            if((L1 << to) & atsq ) atsq_value = 0;
            else atsq_value = 0xffffffff;
            AddMove(pos, MOVE(to-8, to, EMPTY, wK, 0), (atsq_value & PieceVal[bK])-PieceVal[bP], list);
            AddMove(pos, MOVE(to-8, to, EMPTY, wR, 0), (atsq_value & PieceVal[bR])-PieceVal[bP], list);
            AddMove(pos, MOVE(to-8, to, EMPTY, wN, 0), (atsq_value & PieceVal[bN])-PieceVal[bP], list);
            AddMove(pos, MOVE(to-8, to, EMPTY, wB, 0), (atsq_value & PieceVal[bB])-PieceVal[bP], list);
            AddMove(pos, MOVE(to-8, to, EMPTY, wQ, 0), (atsq_value & PieceVal[bQ])-PieceVal[bP], list);
        }

        bb = pos->bitboards[wK];
        while(bb){
            from = PopBit(&bb);
            bbto = KingSquares(L1 << from) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wK];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[wN];
        while(bb){
            from = PopBit(&bb);
            bbto = KnightSquares(L1 << from) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wN];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[wR];
        while(bb){
            from = PopBit(&bb);
            bbto = RookSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wR];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[wB];
        while(bb){
            from = PopBit(&bb);
            bbto = BishopSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wB];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[wQ];
        while(bb){
            from = PopBit(&bb);
            bbto = (BishopSquares(from, pos->occupied[BOTH]) | RookSquares(from, pos->occupied[BOTH])) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wQ];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

    }else{
        bbto = BPawnMoveSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH]) & (uint64_t)0xffffffffffffff00;
        bb = BPawnMoveSquares(bbto) & (~pos->occupied[BOTH]) & (uint64_t)0x000000ff00000000;
        while(bbto){
            to = PopBit(&bbto);
            if((L1 << to) & atsq ) atsq_value = -PieceVal[wP];
            else atsq_value = 0;
            AddMove(pos, MOVE(to+8, to, EMPTY, EMPTY, 0), atsq_value, list);
        }
        while(bb){
            to = PopBit(&bb);
            if((L1 << to) & atsq ) atsq_value = -PieceVal[wP];
            else atsq_value = 0;
            AddMove(pos, MOVE(to+16, to, EMPTY, EMPTY, MFLAGPS), atsq_value, list);
        }

        bbto = BPawnMoveSquares(pos->bitboards[bP]) & (~pos->occupied[BOTH]) & (uint64_t)0x00000000000000ff;
        while(bbto){
            to = PopBit(&bbto);
            if((L1 << to) & atsq ) atsq_value = 0;
            else atsq_value = 0xffffffff;
            AddMove(pos, MOVE(to+8, to, EMPTY, bK, 0), (atsq_value & PieceVal[bK])-PieceVal[bP], list);
            AddMove(pos, MOVE(to+8, to, EMPTY, bR, 0), (atsq_value & PieceVal[bR])-PieceVal[bP], list);
            AddMove(pos, MOVE(to+8, to, EMPTY, bN, 0), (atsq_value & PieceVal[bN])-PieceVal[bP], list);
            AddMove(pos, MOVE(to+8, to, EMPTY, bB, 0), (atsq_value & PieceVal[bB])-PieceVal[bP], list);
            AddMove(pos, MOVE(to+8, to, EMPTY, bQ, 0), (atsq_value & PieceVal[bQ])-PieceVal[bP], list);
        }

        bb = pos->bitboards[bK];
        while(bb){
            from = PopBit(&bb);
            bbto = KingSquares(L1 << from) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wK];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[bN];
        while(bb){
            from = PopBit(&bb);
            bbto = KnightSquares(L1 << from) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wN];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[bR];
        while(bb){
            from = PopBit(&bb);
            bbto = RookSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wR];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[bB];
        while(bb){
            from = PopBit(&bb);
            bbto = BishopSquares(from, pos->occupied[BOTH]) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wB];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }

        bb = pos->bitboards[bQ];
        while(bb){
            from = PopBit(&bb);
            bbto = (BishopSquares(from, pos->occupied[BOTH]) | RookSquares(from, pos->occupied[BOTH])) & (~pos->occupied[BOTH]);
            while(bbto){
                to = PopBit(&bbto);
                if((L1 << to) & atsq ) atsq_value = -PieceVal[wQ];
                else atsq_value = 0;
                AddMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), atsq_value, list);
            }
        }
    }
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list){
    uint64_t bb;
    list->count = 0;
    if(pos->side == WHITE){
        if((pos->enPas != NO_SQ) && (bb = BPawnEatSquares(L1 << pos->enPas) & pos->bitboards[wP])){
            int from;
            while(bb){
                from = PopBit(&bb);
                AddMove(pos, MOVE(from, pos->enPas, bP, EMPTY, MFLAGEP), -100, list);
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
                AddMove(pos, MOVE(from, pos->enPas, wP, EMPTY, MFLAGEP), -100, list);
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
