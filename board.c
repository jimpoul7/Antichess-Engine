#include "defs.h"

char PceChar[] = ".PNBRQKpnbrqk";
char SideChar[] = "wb-";
char RankChar[] = "12345678";
char FileChar[] = "abcdefgh";

int PieceVal[13]= { 0, -250, -250, -450, -400, -300, -50, -250, -250, -450, -400, -350, -50  };

int PieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

void UpdateListsMaterial(S_BOARD *pos) {

    int piece,sq,index,colour;

    for(index = 0; index < BRD_SQ_NUM; ++index) {
        sq = index;
        piece = pos->pieces[index];

        if(piece!=OFFBOARD && piece!= EMPTY) {
            colour = PieceCol[piece];
            pos->material[colour] += PieceVal[piece];
            pos->pceNum[piece]++;


            SETBIT(pos->bitboards[piece],sq);
            SETBIT(pos->occupied[BOTH],sq);
            SETBIT(pos->occupied[PieceCol[piece]],sq);
        }
    }
}

int ParseFen(char *fen, S_BOARD *pos) {

    assert(fen!=NULL);
    assert(pos!=NULL);

    int  rank = RANK_8;
    int  file = FILE_A;
    int  piece = 0;
    int  count = 0;
    int  i = 0;
    int  sq64 = 0;

    ResetBoard(pos);

    while ((rank >= RANK_1) && *fen) {
        count = 1;
        switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            piece = EMPTY;
            count = *fen - '0';
            break;

            case '/':
            case ' ':
            rank--;
            file = FILE_A;
            fen++;
            continue;

            default:
            printf("FEN error \n");
            return -1;
        }

        for (i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            if (piece != EMPTY) {
                pos->pieces[sq64] = piece;
            }
            file++;
        }
        fen++;
    }

    if(*fen != 'w' && *fen != 'b'){
        printf("FEN error \n");
        return -1;
    }

    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;

    for (i = 0; i < 4; i++) {
        if (*fen == ' ') {
            break;
        }
        fen++;
    }
    fen++;

    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';

        assert(file>=FILE_A && file <= FILE_H);
        assert(rank>=RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file,rank);
    }

    pos->posKey = GeneratePosKey(pos);

    UpdateListsMaterial(pos);

    return 0;
}

void ResetBoard(S_BOARD *pos) {

    int index = 0;

    for(index = 0; index < BRD_SQ_NUM; ++index) {
        pos->pieces[index] = OFFBOARD;
    }

    for(index = 0; index < 64; ++index) {
        pos->pieces[index] = EMPTY;
    }

    for(index = 0; index < 3; ++index) {
        pos->occupied[index] = 0ULL;
    }
    for(index = 0; index < 13; ++index) {
        pos->pceNum[index] = 0;
        pos->bitboards[index] = 0;
    }

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply = 0;
    pos->age = 0;
    pos->hisPly = 0;

    pos->posKey = 0ULL;


    for(int j = 0; j < 2; j++) {
        for(int k = 0; k < MAXGAMEMOVES; k++) {
            pos->captureKillers[j][k] = 0;
            pos->quietKillers[j][k] = 0;
        }
    }

    pos->HashTable = malloc(sizeof(S_HASHTABLE));
    InitHashTable(pos->HashTable, 2048);

}

void PrintBoard(const S_BOARD *pos) {

    int sq,file,rank,piece;

    printf("\nGame Board:\n\n");

    for(rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d  ",rank+1);
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file,rank);
            piece = pos->pieces[sq];
            printf("%3c",PceChar[piece]);
        }
        printf("\n");
    }

    printf("\n   ");
    for(file = FILE_A; file <= FILE_H; file++) {
        printf("%3c",'a'+file);
    }
    printf("\n\n");
    printf("Side:%c\n",SideChar[pos->side]);
    printf("enPas:%d\n",pos->enPas);
    printf("PosKey:%lX\n\n",pos->posKey);
}

int IsRepetition(const S_BOARD *pos) {

    int i = 0;

    for(i = pos->hisPly - pos->fiftyMove; i < pos->hisPly-1; i++) {

        if(pos->posKey == pos->history[i].posKey) {
            return 1;
        }
    }
    return 0;
}
