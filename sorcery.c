#include "defs.h"

uint64_t OccMaskR[64];

uint64_t ShiftR[64]=
{
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 53, 53, 53, 53, 53
};

uint64_t ShiftB[64]=
{
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

uint64_t MagicsR[64]=
{
    0x0080001020400080, 0x0040001000200040, 0x0080081000200080, 0x0080040800100080,
    0x0080020400080080, 0x0080010200040080, 0x0080008001000200, 0x0080002040800100,
    0x0000800020400080, 0x0000400020005000, 0x0000801000200080, 0x0000800800100080,
    0x0000800400080080, 0x0000800200040080, 0x0000800100020080, 0x0000800040800100,
    0x0000208000400080, 0x0000404000201000, 0x0000808010002000, 0x0000808008001000,
    0x0000808004000800, 0x0000808002000400, 0x0000010100020004, 0x0000020000408104,
    0x0000208080004000, 0x0000200040005000, 0x0000100080200080, 0x0000080080100080,
    0x0000040080080080, 0x0000020080040080, 0x0000010080800200, 0x0000800080004100,
    0x0000204000800080, 0x0000200040401000, 0x0000100080802000, 0x0000080080801000,
    0x0000040080800800, 0x0000020080800400, 0x0000020001010004, 0x0000800040800100,
    0x0000204000808000, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
    0x0000040008008080, 0x0000020004008080, 0x0000010002008080, 0x0000004081020004,
    0x0000204000800080, 0x0000200040008080, 0x0000100020008080, 0x0000080010008080,
    0x0000040008008080, 0x0000020004008080, 0x0000800100020080, 0x0000800041000080,
    0x00FFFCDDFCED714A, 0x007FFCDDFCED714A, 0x003FFFCDFFD88096, 0x0000040810002101,
    0x0001000204080011, 0x0001000204000801, 0x0001000082000401, 0x0001FFFAABFAD1A2
};

uint64_t MagicsB[64]=
{
    0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000,
    0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
    0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000,
    0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
    0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000,
    0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
    0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200,
    0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
    0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080,
    0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
    0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800,
    0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
    0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000,
    0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
    0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800,
    0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200
};

uint64_t OccMaskB[64]=
{
    0x0040201008040200, 0x0000402010080400, 0x0000004020100A00, 0x0000000040221400,
    0x0000000002442800, 0x0000000204085000, 0x0000020408102000, 0x0002040810204000,
    0x0020100804020000, 0x0040201008040000, 0x00004020100A0000, 0x0000004022140000,
    0x0000000244280000, 0x0000020408500000, 0x0002040810200000, 0x0004081020400000,
    0x0010080402000200, 0x0020100804000400, 0x004020100A000A00, 0x0000402214001400,
    0x0000024428002800, 0x0002040850005000, 0x0004081020002000, 0x0008102040004000,
    0x0008040200020400, 0x0010080400040800, 0x0020100A000A1000, 0x0040221400142200,
    0x0002442800284400, 0x0004085000500800, 0x0008102000201000, 0x0010204000402000,
    0x0004020002040800, 0x0008040004081000, 0x00100A000A102000, 0x0022140014224000,
    0x0044280028440200, 0x0008500050080400, 0x0010200020100800, 0x0020400040201000,
    0x0002000204081000, 0x0004000408102000, 0x000A000A10204000, 0x0014001422400000,
    0x0028002844020000, 0x0050005008040200, 0x0020002010080400, 0x0040004020100800,
    0x0000020408102000, 0x0000040810204000, 0x00000A1020400000, 0x0000142240000000,
    0x0000284402000000, 0x0000500804020000, 0x0000201008040200, 0x0000402010080400,
    0x0002040810204000, 0x0004081020400000, 0x000A102040000000, 0x0014224000000000,
    0x0028440200000000, 0x0050080402000000, 0x0020100804020000, 0x0040201008040200
};


uint64_t MagicDBR[64][0x1000];
uint64_t MagicDBB[64][0x1000];

void InitOccMasks(){
    uint64_t shift = 1;
    int rank, file;
    for(int i = 0; i < 64; i++){
        rank = i/8;
        file = i%8;
        OccMaskR[i] = 0;
        //OccMaskB[i] = 0;
        for(int j = 1; j < 7; j++){
            if(j == file) continue;
            OccMaskR[i] |= (shift << (8*rank + j));
        }
        for(int j = 1; j < 7; j++){
            if(j == rank) continue;
            OccMaskR[i] |= (shift << (8*j + file));
        }
    }
}

uint64_t initmagicmoves_occ(const int* squares, int numSquares, uint64_t linocc)
{
    int i;
    uint64_t ret=0;
    for(i=0;i<numSquares;i++)
    if(linocc&(((uint64_t)(1))<<i)) ret|=(((uint64_t)(1))<<squares[i]);
    return ret;
}

uint64_t initmagicmoves_Rmoves(const int square, uint64_t occ)
{
    uint64_t ret=0;
    uint64_t bit;

    //for bitscans :
    //initmagicmoves_bitpos64_database[(x*C64(0x07EDD5E59A4E28C2))>>58

    uint64_t rowbits=(((uint64_t)0xFF)<<(8*(square/8)));

    bit=(((uint64_t)(1))<<square);
    do
    {
        bit<<=8;
        ret|=bit;
    }while(bit && !(bit&occ));
    bit=(((uint64_t)(1))<<square);
    do
    {
        bit>>=8;
        ret|=bit;
    }while(bit && !(bit&occ));
    bit=(((uint64_t)(1))<<square);
    do
    {
        bit<<=1;
        if(bit&rowbits) ret|=bit;
        else break;
    }while(!(bit&occ));
    bit=(((uint64_t)(1))<<square);
    do
    {
        bit>>=1;
        if(bit&rowbits) ret|=bit;
        else break;
    }while(!(bit&occ));

    return ret;
}

uint64_t initmagicmoves_Bmoves(const int square, uint64_t occ)
{
    uint64_t ret=0;
    uint64_t bit;
    uint64_t bit2;
    uint64_t rowbits=(((uint64_t)0xFF)<<(8*(square/8)));

    bit=(((uint64_t)(1))<<square);
    bit2=bit;
    do
    {
        bit<<=8-1;
        bit2>>=1;
        if(bit2&rowbits) ret|=bit;
        else break;
    }while(bit && !(bit&occ));
    bit=(((uint64_t)(1))<<square);
    bit2=bit;
    do
    {
        bit<<=8+1;
        bit2<<=1;
        if(bit2&rowbits) ret|=bit;
        else break;
    }while(bit && !(bit&occ));
    bit=(((uint64_t)(1))<<square);
    bit2=bit;
    do
    {
        bit>>=8-1;
        bit2<<=1;
        if(bit2&rowbits) ret|=bit;
        else break;
    }while(bit && !(bit&occ));
    bit=(((uint64_t)(1))<<square);
    bit2=bit;
    do
    {
        bit>>=8+1;
        bit2>>=1;
        if(bit2&rowbits) ret|=bit;
        else break;
    }while(bit && !(bit&occ));
    return ret;
}

void InitDB(){

    int initmagicmoves_bitpos64_database[64]={
        63,  0, 58,  1, 59, 47, 53,  2,
        60, 39, 48, 27, 54, 33, 42,  3,
        61, 51, 37, 40, 49, 18, 28, 20,
        55, 30, 34, 11, 43, 14, 22,  4,
        62, 57, 46, 52, 38, 26, 32, 41,
        50, 36, 17, 19, 29, 10, 13, 21,
        56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5};

    for(int i=0;i<64;i++)
    {
        int squares[64];
        int numsquares=0;
        uint64_t temp=OccMaskR[i];
        while(temp)
        {
            uint64_t bit=temp&-temp;
            squares[numsquares++]=initmagicmoves_bitpos64_database[(bit*0x07EDD5E59A4E28C2)>>58];
            temp^=bit;
        }
        for(temp=0;temp<(((uint64_t)(1))<<numsquares);temp++)
        {
            uint64_t tempocc=initmagicmoves_occ(squares,numsquares,temp);
            MagicDBR[i][((tempocc)*MagicsR[i])>>ShiftR[i]]=initmagicmoves_Rmoves(i,tempocc);
        }
    }

    for(int i=0;i<64;i++)
    {
        int squares[64];
        int numsquares=0;
        uint64_t temp=OccMaskB[i];
        while(temp)
        {
            uint64_t bit=temp&-temp;
            squares[numsquares++]=initmagicmoves_bitpos64_database[(bit*0x07EDD5E59A4E28C2)>>58];
            temp^=bit;
        }
        for(temp=0;temp<(((uint64_t)(1))<<numsquares);temp++)
        {
            uint64_t tempocc=initmagicmoves_occ(squares,numsquares,temp);
            MagicDBB[i][((tempocc)*MagicsB[i])>>ShiftB[i]]=initmagicmoves_Bmoves(i,tempocc);
        }
    }

}

void InitSorcery(){
    InitOccMasks();
    InitDB();
}

uint64_t RookSquares(int square, uint64_t occupied){
    return MagicDBR[square][(((occupied)&OccMaskR[square])*MagicsR[square])>>ShiftR[square]];
}

uint64_t BishopSquares(int square, uint64_t occupied){
    return MagicDBB[square][(((occupied)&OccMaskB[square])*MagicsB[square])>>ShiftB[square]];
}
