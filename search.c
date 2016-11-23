#include "defs.h"

int minimax(S_BOARD *pos, int depth){

  if(pos->fiftyMove >= 100) return 0;

  S_MOVELIST list;
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
			int best = -300000000, cur;
			for(int i = 0; i < num_of_moves; i++){
        move = list.moves[i].move;
    		MakeMove(pos, move);
				cur = minimax(pos,depth-1);
				if(cur > best) best = cur;
				UndoMove(pos);
			}
      return best;
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
			int best = 300000000, cur;
			for(int i = 0; i < num_of_moves; i++){
        move = list.moves[i].move;
    		MakeMove(pos, move);
				cur = minimax(pos,depth-1);
				if(cur < best) best = cur;
				UndoMove(pos);
			}
			return best;
    }
  }
}

S_MOVE FindMove(S_BOARD *pos, int depth){

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
				cur = minimax(pos,depth-1);
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
				cur = minimax(pos,depth-1);
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
