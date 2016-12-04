#include "defs.h"
#include <time.h>

long int nodes = 0;

int minimax(S_BOARD *pos, int depth, int a, int b){

    nodes++;

    __builtin_prefetch(pos->HashTable->pTable+(pos->posKey % pos->HashTable->numofEntries));

    if(pos->fiftyMove >= 100) return 0;
    //if(IsRepetition(pos)) return 0;

    int bestmove = 0;
    S_MOVELIST list;
    int hashmove;
    GenerateAllMoves(pos,&list);
    OrderMoves(&list);

    if( ProbeHashEntry(pos, &hashmove, &bestmove, a, b, depth) == 1 ) {
        pos->HashTable->cut++;
        return bestmove;
    }
    else{
        bestmove = 0;
    }
    int num_of_moves = list.count;
    int move = 0;
    if(pos->side == WHITE){
        if(num_of_moves == 0){
            return 200000000;
        }
        else if(depth <= 0){
            //return Quiescence(pos,a,b);
            return Eval(pos);
        }
        else{
            int cur;
            for(int i = 0; i < num_of_moves; i++){
                move = list.moves[i].move;
                MakeMove(pos, move);
                cur = minimax(pos,depth-1,a,b);
                if(cur > 100000000) cur--;
                if(cur < -100000000) cur++;
                if(cur >= b){
                    UndoMove(pos);
                    if(CAPTURED(move)){
                        pos->captureKillers[1][pos->ply] = pos->captureKillers[0][pos->ply];
                        pos->captureKillers[0][pos->ply] = move;
                    }
                    else{
                        pos->quietKillers[1][pos->ply] = pos->quietKillers[0][pos->ply];
                        pos->quietKillers[0][pos->ply] = move;
                    }
                    StoreHashEntry(pos, move, b, HFBETA, depth);
                    return b;
                }
                if(cur > a){
                    a = cur;
                    bestmove = move;
                }
                UndoMove(pos);
            }
            if(bestmove){
                StoreHashEntry(pos, bestmove, a, HFEXACT, depth);
            }
            else{
                StoreHashEntry(pos, NOMOVE, a, HFALPHA, depth);
            }
            return a;
        }
    }
    else{
        if(num_of_moves == 0){
            return -200000000;
        }
        else if(depth <= 0){
            return Eval(pos);
        }
        else{
            int cur;
            for(int i = 0; i < num_of_moves; i++){
                move = list.moves[i].move;
                MakeMove(pos, move);
                cur = minimax(pos,depth-1,a,b);
                if(cur > 100000000) cur--;
                if(cur < -100000000) cur++;
                if(cur <= a){
                    UndoMove(pos);
                    if(CAPTURED(move)){
                        pos->captureKillers[1][pos->ply] = pos->captureKillers[0][pos->ply];
                        pos->captureKillers[0][pos->ply] = move;
                    }
                    else{
                        pos->quietKillers[1][pos->ply] = pos->quietKillers[0][pos->ply];
                        pos->quietKillers[0][pos->ply] = move;
                    }
                    StoreHashEntry(pos, move, a, HFALPHA, depth);
                    return a;
                }
                if(cur < b){
                    b = cur;
                    bestmove = move;
                }
                UndoMove(pos);
            }
            if(bestmove){
                StoreHashEntry(pos, bestmove, b, HFEXACT, depth);
            }
            else{
                StoreHashEntry(pos, NOMOVE, b, HFBETA, depth);
            }
            return b;
        }
    }
}

int Quiescence(S_BOARD *pos, int a, int b){

    nodes++;

    __builtin_prefetch(pos->HashTable->pTable+(pos->posKey % pos->HashTable->numofEntries));

    if(pos->fiftyMove >= 100) return 0;

    int bestmove = 0;
    S_MOVELIST Allmoves;
    S_MOVELIST caps;
    int hashmove;
    int move = 0;
    int j = 0;
    int num_of_caps = 0;
    int score;

    GenerateAllMoves(pos,&Allmoves);

    if(ProbeHashEntry(pos, &hashmove, &bestmove, a, b, 0) == 1) {
        pos->HashTable->cut++;
        return bestmove;
    }
    else{
        bestmove = 0;
    }

    for(int i = 0; i < Allmoves.count; i++){
        if(CAPTURED(move)){
            caps.moves[j].move = move;
            caps.count++;
        }
    }

    if(pos->side == WHITE){
        if(!Allmoves.count){
            return 200000000;
        }
        else if(!num_of_caps){
            return Eval(pos);
        }
        else{ // Check captures
            OrderMoves(&caps);
            for(int i = 0; i < num_of_caps; i++){
                move = caps.moves[i].move;
                MakeMove(pos, move);
                score = Quiescence(pos,a,b);
                if(score > 100000000) score--;
                if(score < -100000000) score++;
                if(score >= b){
                    UndoMove(pos);
                    pos->captureKillers[1][pos->ply] = pos->captureKillers[0][pos->ply];
                    pos->captureKillers[0][pos->ply] = move;
                    StoreHashEntry(pos, move, b, HFBETA, 0);
                    return b;
                }
                if(score > a){
                    a = score;
                    bestmove = move;
                }
                UndoMove(pos);
            }
            if(bestmove){
                StoreHashEntry(pos, bestmove, a, HFEXACT, 0);
            }
            else{
                StoreHashEntry(pos, NOMOVE, a, HFALPHA, 0);
            }
            return a;
        }
    }
    else{
        if(!Allmoves.count){
            return -200000000;
        }
        else if(!num_of_caps){
            return Eval(pos);
        }
        else{ // Check captures
            OrderMoves(&caps);
            for(int i = 0; i < num_of_caps; i++){
                move = caps.moves[i].move;
                MakeMove(pos, move);
                score = Quiescence(pos,a,b);
                if(score > 100000000) score--;
                if(score < -100000000) score++;
                if(score <= a){
                    UndoMove(pos);
                    pos->captureKillers[1][pos->ply] = pos->captureKillers[0][pos->ply];
                    pos->captureKillers[0][pos->ply] = move;
                    StoreHashEntry(pos, move, a, HFALPHA, 0);
                    return a;
                }
                if(score < b){
                    b = score;
                    bestmove = move;
                }
                UndoMove(pos);
            }
            if(bestmove){
                StoreHashEntry(pos, bestmove, b, HFEXACT, 0);
            }
            else{
                StoreHashEntry(pos, NOMOVE, b, HFBETA, 0);
            }
            return b;
        }
    }

}

S_MOVE FindBestMove(S_BOARD *pos, int depth){

    S_MOVELIST list;
    GenerateAllMoves(pos,&list);
    OrderMoves(&list);
    int num_of_moves = list.count;
    int move = 0,best_move=0;
    S_MOVE bm;

    if(pos->side == WHITE){
        if(num_of_moves == 0){
            bm.score = 200000000;
            bm.move = 0;
            return bm;
        }
        else{
            int best = -300000000, cur;
            for(int i = 0; i < num_of_moves; i++){
                move = list.moves[i].move;
                MakeMove(pos, move);
                cur = minimax(pos,depth-1,best,300000000);
                if(cur > best){
                    best = cur;
                    best_move = move;
                }
                UndoMove(pos);
            }
            bm.score = best;
            bm.move = best_move;
            StoreHashEntry(pos, best_move, best, HFEXACT, depth);
            return bm;
        }
    }
    else{
        if(num_of_moves == 0){
            bm.score = -200000000;
            bm.move = 0;
            return bm;
        }
        else{
            int best = 300000000, cur;
            for(int i = 0; i < num_of_moves; i++){
                move = list.moves[i].move;
                MakeMove(pos, move);
                cur = minimax(pos,depth-1,-300000000,best);
                if(cur < best){
                    best = cur;
                    best_move = move;
                }
                UndoMove(pos);
            }
            bm.score = best;
            bm.move = best_move;
            StoreHashEntry(pos, best_move, best, HFEXACT, depth);
            return bm;
        }
    }

}

S_MOVE Ids(S_BOARD *pos, int depth) {

    S_MOVE move;

    for(int i=1; i<=depth; i++){
        move = FindBestMove(pos,i);
        if((move.move != NOMOVE) && (i > depth-3)) printf("Depth: %d Best move: %s\n",i,PrMove2(pos, move.move));
    }

    return move;

}

int FindMoves(S_BOARD *pos, int depth){

    S_MOVELIST list;
    //PrintBoard(pos);
    GenerateAllMoves(pos,&list);
    //printf("%d\n", depth);
    //PrintMoveList(&list);
    //printf("%x\n", list.moves[0].move);
    int num_of_moves = list.count;
    int move = 0;
    int total = 0;

    if(num_of_moves == 0) return 1;

    if(depth <= 0){
        return 1;
    }
    for(int i = 0; i < num_of_moves; i++){
        move = list.moves[i].move;
        MakeMove(pos, move);
        total += FindMoves(pos,depth-1);
        UndoMove(pos);
    }
    return total;
}

void OrderMoves(S_MOVELIST *list){

    S_MOVE tmp;
    for(int i = 1; i < list->count; i++){
        int j = i;
        while (j > 0 && list->moves[j-1].score < list->moves[j].score){
            tmp = list->moves[j];
            list->moves[j] = list->moves[j-1];
            list->moves[j-1] = tmp;
            j--;
        }
    }
}

void PrintPvline(S_BOARD *pos){

    int move,index;
    uint64_t tag;

    index = pos->posKey % pos->HashTable->numofEntries;
    tag = pos->HashTable->pTable[index].posKey;
    move = pos->HashTable->pTable[index].move;

    if(move && tag == pos->posKey && !IsRepetition(pos)){
        if(pos->side == BLACK) printf("...%s ",PrMove2(pos, move));
        else printf("%d. %s",pos->ply/2+1, PrMove2(pos, move));
        MakeMove(pos,move);
        PrintPvline(pos);
        UndoMove(pos);
    }

}
