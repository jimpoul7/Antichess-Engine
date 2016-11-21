#include "defs.h"

int main() {

	AllInit();

	S_BOARD board[1];
	S_MOVELIST list[1];

	ParseFen("8/8/8/1p6/Pp6/2Q5/1p6/Q7 b - a3",board);
	GenerateAllMoves(board,list);

	int MoveNum = 0;
	int move = 0;

	PrintBoard(board);
	getchar();

	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		move = list->moves[MoveNum].move;

		MakeMove(board, move);

		printf("\nMADE:%s\n", PrMove(move));
		PrintBoard(board);

		UndoMove(board);
		printf("\nTAKEN:%s\n", PrMove(move));
		PrintBoard(board);

		getchar();
	}

	return 0;
}
