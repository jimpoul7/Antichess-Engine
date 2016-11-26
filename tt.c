#include "defs.h"

void ClearHashTable(S_HASHTABLE *table) {

  S_HASHENTRY *tableEntry;

  for (tableEntry = table->pTable; tableEntry < table->pTable + table->numofEntries; tableEntry++) {
    tableEntry->posKey = 0ULL;
    //tableEntry->move = NOMOVE;
    tableEntry->depth = 0;
    tableEntry->score = 0;
    tableEntry->flags = 0;
  }
  table->newWrite=0;
}

void InitHashTable(S_HASHTABLE *table, int MB) {
	int HashSize = 0x100000 * MB;
    table->numofEntries = HashSize / sizeof(S_HASHENTRY);
    table->numofEntries -= 2;
    if(table->pTable) {
    	//free(table->pTable);
    }
    table->pTable = (S_HASHENTRY *) malloc(table->numofEntries * sizeof(S_HASHENTRY));
    ClearHashTable(table);

}

void StoreHashEntry(S_BOARD *pos, int score, int flags, int depth){

  int index = pos->posKey % pos->HashTable->numofEntries;

	if( pos->HashTable->pTable[index].posKey == 0) {
		pos->HashTable->newWrite++;
	} else {
		pos->HashTable->overWrite++;
	}

	//if(score > ISMATE) score += pos->ply;
  //else if(score < -ISMATE) score -= pos->ply;

	//pos->HashTable->pTable[index].move = move;
  pos->HashTable->pTable[index].posKey = pos->posKey;
	pos->HashTable->pTable[index].flags = flags;
	pos->HashTable->pTable[index].score = score;
	pos->HashTable->pTable[index].depth = depth;
}

int ProbeHashEntry(S_BOARD *pos, int *score, int alpha, int beta, int depth) {

	int index = pos->posKey % pos->HashTable->numofEntries;

	if( pos->HashTable->pTable[index].posKey == pos->posKey ) {
		//*move = pos->HashTable->pTable[index].move;
		if(pos->HashTable->pTable[index].depth >= depth){
			pos->HashTable->hit++;
			*score = pos->HashTable->pTable[index].score;
			//if(*score > ISMATE) *score -= pos->ply;
            //else if(*score < -ISMATE) *score += pos->ply;

			switch(pos->HashTable->pTable[index].flags) {

                case HFALPHA: if(*score<=alpha) {
                    *score=alpha;
                    return 1;
                    }
                    break;
                case HFBETA: if(*score>=beta) {
                    *score=beta;
                    return 1;
                    }
                    break;
                case HFEXACT:
                    return 1;
                    break;
                default: break;
            }
		}
	}

	return 0;
}
