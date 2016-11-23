#include "defs.h"

int main() {

	AllInit();

	S_BOARD pos;
	S_MOVE move;

	ParseFen(START_FEN,&pos);

	while(1){
		if(pos.fiftyMove >= 100){
			printf("Sergey\n");
			break;
		}
		if(pos.side == WHITE){
			move = FindMove(&pos,3);
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
			 printf("gg\n");
			 break;
		}
		MakeMove(&pos,move.move);
		PrintBoard(&pos);
	}

	return 0;
}
