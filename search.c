#include "defs.h"

long int nodes = 0;

int minimax(S_BOARD *pos, int depth, int a, int b){

  nodes++;

  if(pos->fiftyMove >= 100) return 0;

  int abchanged = 0;
  S_MOVELIST list;

  if( ProbeHashEntry(pos, &abchanged, a, b, depth) == 1 ) {
		pos->HashTable->cut++;
		return abchanged;
	}
  else{
    abchanged = 0;
  }
  GenerateAllMoves(pos,&list);
  int num_of_moves = list.count;
  int move = 0;

	if(pos->side == WHITE){
		if(num_of_moves == 0){
			return 200000000;
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
        if(cur >= b){
          UndoMove(pos);
          StoreHashEntry(pos, b, HFBETA, depth);
          return b;
        }
				if(cur > a){
           a = cur;
           abchanged = 1;
        }
				UndoMove(pos);
			}
      if(abchanged){
        StoreHashEntry(pos, a, HFEXACT, depth);
      }
      else{
        StoreHashEntry(pos, a, HFALPHA, depth);
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
          StoreHashEntry(pos, a, HFALPHA, depth);
          return a;
        }
				if(cur < b){
           b = cur;
           abchanged = 1;
        }
				UndoMove(pos);
			}
      if(abchanged){
        StoreHashEntry(pos, b, HFEXACT, depth);
      }
      else{
        StoreHashEntry(pos, b, HFBETA, depth);
      }
			return b;
    }
  }
}

S_MOVE FindBestMove(S_BOARD *pos, int depth){

  S_MOVELIST list;
  GenerateAllMoves(pos,&list);
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
      return bm;
    }
  }

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
