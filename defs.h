#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define NAME "TBD"
#define BRD_SQ_NUM 64

#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64
#define MAX_HASH 1024

#define START_FEN  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1"

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK  };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum { WHITE, BLACK, BOTH };
//enum { UCIMODE };
enum {
  A1 = 0, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { HFNONE, HFALPHA, HFBETA, HFEXACT};

typedef struct {
	uint64_t posKey;
	//int move;
	int score;
	int depth;
	int flags;
} S_HASHENTRY;

typedef struct {
	S_HASHENTRY *pTable;
	int numofEntries;
	int newWrite;
	int overWrite;
	int hit;
	int cut;
} S_HASHTABLE;

typedef struct {

	int move;
	int score;

} S_MOVE;

typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
  int capture;
} S_MOVELIST;

typedef struct {

  int move;
  int fiftyMove;
  int enPas;
  uint64_t posKey;

} S_UNDO;

typedef struct {

  int pieces[BRD_SQ_NUM];

  uint64_t occupied[3];
  uint64_t bitboards[13];

  int side;
  int enPas;
  int fiftyMove;

  int ply;
  int hisPly;

  uint64_t posKey;

  int pceNum[13];
  int material[2];

  S_UNDO history[MAXGAMEMOVES];
  S_HASHTABLE *HashTable;

} S_BOARD;

typedef struct {

	int starttime;
	int stoptime;
	int depth;
	int timeset;
	int movestogo;

	long nodes;

	int quit;
	int stopped;

	float fh;
	float fhf;
	int nullCut;

	int GAME_MODE;
	int POST_THINKING;

} S_SEARCHINFO;

/* GAME MOVE */

/*
0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
*/

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define NOMOVE 0

/* MACROS */

#define FR2SQ(f,r) ( 8*r+f )
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb,sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb,sq) ((bb) |= SetMask[(sq)])

/* GLOBALS */

extern uint64_t SetMask[64];
extern uint64_t ClearMask[64];
extern uint64_t PieceKeys[13][64];
extern uint64_t SideKey;
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern int PieceVal[13];
extern int PieceCol[13];
extern long int nodes;

/* FUNCTIONS */

// init.c
extern void AllInit();

// bitboards.c
extern void PrintBitBoard(uint64_t bb);
extern int PopBit(uint64_t *bb);
extern int CountBits(uint64_t b);
extern uint64_t KingSquares(uint64_t b);
extern uint64_t KnightSquares(uint64_t b);
extern uint64_t WPawnEatSquares(uint64_t b);
extern uint64_t BPawnEatSquares(uint64_t b);
extern uint64_t WPawnMoveSquares(uint64_t b);
extern uint64_t BPawnMoveSquares(uint64_t b);
extern uint64_t WPawnJumpSquares(uint64_t b);
extern uint64_t BPawnJumpSquares(uint64_t b);

//board.c
extern void ResetBoard(S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern int  ParseFen(char *fen, S_BOARD *pos);
void UpdateListsMaterial(S_BOARD *pos);

//hashkeys.c
extern uint64_t GeneratePosKey(const S_BOARD *pos);

// move.c
extern char *PrMove(const int move);
extern char *PrSq(const int sq);
extern void PrintMoveList(const S_MOVELIST *list);
extern int ParseMove(char *ptrChar, S_BOARD *pos);

// movegen.c
void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
uint64_t CaptureSquares(const S_BOARD *pos, int side);

// makemove.c
extern void MakeMove(S_BOARD *pos, int move);
extern void UndoMove(S_BOARD *pos);

//evaluation.c
extern int Eval(S_BOARD *pos);

//search.c
extern S_MOVE FindBestMove(S_BOARD *pos, int depth);
extern int FindMoves(S_BOARD *pos, int i);
extern int IsRepetition(const S_BOARD *pos);
extern void OrderMoves(S_MOVELIST *list);

//uci.c
//extern void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos);
//extern void ParsePosition(char* lineIn, S_BOARD *pos);
//extern void Uci_Loop(S_BOARD *pos, S_SEARCHINFO *info);

//Sorcery.c
extern void InitSorcery();
extern uint64_t RookSquares(int square, uint64_t occupied);
extern uint64_t BishopSquares(int square, uint64_t occupied);

//tt.c
extern void InitHashTable(S_HASHTABLE *table, int MB);
extern int ProbeHashEntry(S_BOARD *pos, int *score, int alpha, int beta, int depth);
extern void StoreHashEntry(S_BOARD *pos, int score, int flags, int depth);

#endif
