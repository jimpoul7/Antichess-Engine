#include "defs.h"
#include <time.h>

void Play(){

    S_BOARD pos;
    S_MOVE move;
    int depth;

    ParseFen(START_FEN,&pos);

    printf("Give depth:");
    scanf("%d",&depth);

    PrintBoard(&pos);

    while(1){
        if(pos.fiftyMove >= 100){
            printf("Sergey\n");
            break;
        }
        if(1){
            S_MOVELIST list;
            GenerateAllMoves(&pos,&list);
            int n;
            if(list.count == 1){
                move = list.moves[0];
            }
            else{
                move = Ids(&pos,depth);
            }
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
                    printf("#%d:\t%s\n",i,PrMove2(&pos, list.moves[i].move));
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
        //PrintPvline(&pos);
        if(pos.side == WHITE) printf("\nWhite played: %s\n",PrMove2(&pos,move.move));
        else printf("\nBlack played: %s\n",PrMove2(&pos,move.move));
            MakeMove(&pos,move.move);
        pos.age++;
        PrintBoard(&pos);
        printf("Eval: %d\n",Eval(&pos));
        printf("Score %d\n", move.score);
        PrintPvline(&pos);
        printf("\n--------------------------------------------------------------------------------------------------------\n");
    }
    printf("%ld Nodes, %ld Hits, %ld Cuts, %ld Overwrites, %ld No-writes\n",nodes,pos.HashTable->hit,pos.HashTable->cut,pos.HashTable->overWrite,pos.HashTable->noWrite);

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
        for(int i = 1; i <= 6; i++){
            start = clock();
            moves = FindMoves(&pos, i);
            //printf("%d ", moves);
            printf("%lf ", ((double)clock()-start)/CLOCKS_PER_SEC);
        }
        printf("\n");
    }
}

int main() {

    AllInit();

    Play();

    //CheckMoveGen();

    // uint64_t bb;
    // S_BOARD pos;
    //
    // ParseFen("rn6/2pp1k2/8/8/1b4P1/3P1P2/2PK4/8 b - - 0 1",&pos);
    //
    // bb = MoveSquares(&pos,BLACK);
    // PrintBitBoard(bb);
    // printf("Mobility: %d\n",Mobility(&pos,BLACK));

    return 0;
}
