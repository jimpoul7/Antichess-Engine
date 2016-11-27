#include "defs.h"
#include <time.h>

void Play(int depth){

	S_BOARD pos;
	S_MOVE move;

	ParseFen(START_FEN,&pos);
	PrintBoard(&pos);

	while(1){
		if(pos.fiftyMove >= 100){
			printf("Sergey\n");
			break;
		}
		if(1){
			move = FindBestMove(&pos,depth);
		}
		else{
			S_MOVELIST list;
			GenerateAllMoves(&pos,&list);
			int n;
			if(list.count == 0){
				move.move = 0;
			}
			else{
				for(int i = 0; i < list.count; i++) {
					printf("#%d:%s\n",i,PrMove(list.moves[i].move));
				}
				printf("Give move:");
				scanf("%d",&n);
				move.move = list.moves[n].move;
			}
		}
		if(move.move == 0){
			 if(pos.side == WHITE) printf("White wins!\n");
			 else printf("Black wins!\n");
			 break;
		}
		MakeMove(&pos,move.move);
		PrintBoard(&pos);
	}
	printf("%ld Nodes,%d Hits, %d Cuts\n ",nodes,pos.HashTable->hit,pos.HashTable->cut);

}

void CheckMoveGen(){

	S_BOARD pos;
	int moves;
	clock_t start;
	FILE *fp1;
	char fen[1024];

	fp1 = fopen("perft.epd", "r");

	for(int k = 0; k < 126; k++){
		//S_MOVELIST list;
		fgets(fen, 1024, fp1);
		ParseFen(fen,&pos);
		//GenerateAllMoves(&pos,&list);
		//PrintMoveList(&list);
		//abort();
		for(int i = 1; i <= 7; i++){
			start = clock();
			moves = FindMoves(&pos, i);
			printf("%d ", moves);
			//printf("%lf ", ((double)clock()-start)/CLOCKS_PER_SEC);
		}
		printf("\n");
	}
}

int main() {

	AllInit();

	Play(14);

	return 0;
}
