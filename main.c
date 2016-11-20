#include "defs.h"

int main(){

  AllInit();

	S_BOARD board[1];

	ParseFen("7N/8/8/8/4K3/8/8/8 w - -",board);
	PrintBoard(board);

	S_MOVELIST list[1];

	GenerateAllMoves(board,list);

	PrintMoveList(list);

	return 0;
}
