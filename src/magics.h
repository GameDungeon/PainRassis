#pragma once

#include "bitboard.h"

const int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

const int BBits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

extern Bitboard BishopAttacks[64][1024];
extern Bitboard RookAttacks  [64][4096];

void initRookMagicTable();
void initBishopMagicTable();

struct Magic {
    Bitboard mask;
    Bitboard magic;
    unsigned int shift;
};

void GenerateMagics();

inline uint64_t getMagicIndex(const Magic &m, Bitboard blockers) {
    return ((blockers * m.magic) >> (64 - m.shift)).bb;
}

// Regenerate with GenerateMagics

const Magic RookMagics[64] = {
  { 0x101010101017eULL, 0x4080015140002480ULL, 12},
  { 0x202020202027cULL, 0x40200040001000ULL, 11},
  { 0x404040404047aULL, 0x5080082000801000ULL, 11},
  { 0x8080808080876ULL, 0x2080040800100081ULL, 11},
  { 0x1010101010106eULL, 0x200040810020021ULL, 11},
  { 0x2020202020205eULL, 0x30009000804000aULL, 11},
  { 0x4040404040403eULL, 0xc80108001003200ULL, 11},
  { 0x8080808080807eULL, 0xb00022201804500ULL, 12},
  { 0x1010101017e00ULL, 0x51002100800040ULL, 11},
  { 0x2020202027c00ULL, 0x106401006200040ULL, 10},
  { 0x4040404047a00ULL, 0x802001020804206ULL, 10},
  { 0x8080808087600ULL, 0x2002000b10220041ULL, 10},
  { 0x10101010106e00ULL, 0xa5d000412480100ULL, 10},
  { 0x20202020205e00ULL, 0x401a000200081044ULL, 10},
  { 0x40404040403e00ULL, 0x4000804021001ULL, 10},
  { 0x80808080807e00ULL, 0x582000042010084ULL, 11},
  { 0x10101017e0100ULL, 0x40108000802045ULL, 11},
  { 0x20202027c0200ULL, 0x3820004030004002ULL, 10},
  { 0x40404047a0400ULL, 0x4001010010402000ULL, 10},
  { 0x8080808760800ULL, 0x142120009220040ULL, 10},
  { 0x101010106e1000ULL, 0x1000050010080100ULL, 10},
  { 0x202020205e2000ULL, 0x24004002010040ULL, 10},
  { 0x404040403e4000ULL, 0x862040041281002ULL, 10},
  { 0x808080807e8000ULL, 0x20020984104ULL, 11},
  { 0x101017e010100ULL, 0x80004040002015ULL, 11},
  { 0x202027c020200ULL, 0x200040005000ULL, 10},
  { 0x404047a040400ULL, 0x110040020002800ULL, 10},
  { 0x8080876080800ULL, 0x800401200220008ULL, 10},
  { 0x1010106e101000ULL, 0x2006004600100820ULL, 10},
  { 0x2020205e202000ULL, 0x500a000404002010ULL, 10},
  { 0x4040403e404000ULL, 0x4020010400105228ULL, 10},
  { 0x8080807e808000ULL, 0x44288200010044ULL, 11},
  { 0x1017e01010100ULL, 0x3400400828800081ULL, 11},
  { 0x2027c02020200ULL, 0x1040402008401000ULL, 10},
  { 0x4047a04040400ULL, 0x820401101002001ULL, 10},
  { 0x8087608080800ULL, 0x330010008080080ULL, 10},
  { 0x10106e10101000ULL, 0x200200a000410ULL, 10},
  { 0x20205e20202000ULL, 0x40c0201008010440ULL, 10},
  { 0x40403e40404000ULL, 0x2040010804001002ULL, 10},
  { 0x80807e80808000ULL, 0x240040042002089ULL, 11},
  { 0x17e0101010100ULL, 0x80004020004000ULL, 11},
  { 0x27c0202020200ULL, 0x2100420100820028ULL, 10},
  { 0x47a0404040400ULL, 0x410020010010ULL, 10},
  { 0x8760808080800ULL, 0x2001000810010020ULL, 10},
  { 0x106e1010101000ULL, 0x2008020004004040ULL, 10},
  { 0x205e2020202000ULL, 0xc000201004040ULL, 10},
  { 0x403e4040404000ULL, 0x81000a0011002cULL, 10},
  { 0x807e8080808000ULL, 0x104100820014ULL, 11},
  { 0x7e010101010100ULL, 0x4410038800100ULL, 11},
  { 0x7c020202020200ULL, 0x50200040100040ULL, 10},
  { 0x7a040404040400ULL, 0x4040200704421100ULL, 10},
  { 0x76080808080800ULL, 0x10200a004200ULL, 10},
  { 0x6e101010101000ULL, 0x300041008010100ULL, 10},
  { 0x5e202020202000ULL, 0x3001000208040100ULL, 10},
  { 0x3e404040404000ULL, 0x200100221480400ULL, 10},
  { 0x7e808080808000ULL, 0x80011040840200ULL, 11},
  { 0x7e01010101010100ULL, 0x4030108201004022ULL, 12},
  { 0x7c02020202020200ULL, 0x410220100104082ULL, 11},
  { 0x7a04040404040400ULL, 0x800104022000882ULL, 11},
  { 0x7608080808080800ULL, 0x601001000204855ULL, 11},
  { 0x6e10101010101000ULL, 0x24200080c205006ULL, 11},
  { 0x5e20202020202000ULL, 0x20200044b900802ULL, 11},
  { 0x3e40404040404000ULL, 0x108201103804ULL, 11},
  { 0x7e80808080808000ULL, 0x2244002410804102ULL, 12},
};

const Magic BishopMagics[64] = {
  { 0x40201008040200ULL, 0x10021014002040ULL, 6},
  { 0x402010080400ULL, 0x512080252820202ULL, 5},
  { 0x4020100a00ULL, 0x1b0248200480020ULL, 5},
  { 0x40221400ULL, 0x404105202404000ULL, 5},
  { 0x2442800ULL, 0x4042004100001ULL, 5},
  { 0x204085000ULL, 0xc2019008400400ULL, 5},
  { 0x20408102000ULL, 0x12083108094004ULL, 5},
  { 0x2040810204000ULL, 0x2003014244044002ULL, 6},
  { 0x20100804020000ULL, 0x204080208082100ULL, 5},
  { 0x40201008040000ULL, 0x4201255002020021ULL, 5},
  { 0x4020100a0000ULL, 0x20040104010800ULL, 5},
  { 0x4022140000ULL, 0x1080841080110ULL, 5},
  { 0x244280000ULL, 0x6084040420000001ULL, 5},
  { 0x20408500000ULL, 0x304009010080010ULL, 5},
  { 0x2040810200000ULL, 0x40220402080404a2ULL, 5},
  { 0x4081020400000ULL, 0x2522228120809ULL, 5},
  { 0x10080402000200ULL, 0x510024410020830ULL, 5},
  { 0x20100804000400ULL, 0x2602000950210200ULL, 5},
  { 0x4020100a000a00ULL, 0x910000800302020ULL, 7},
  { 0x402214001400ULL, 0x1048040c02400903ULL, 7},
  { 0x24428002800ULL, 0x2050400a20080ULL, 7},
  { 0x2040850005000ULL, 0xe0200e10042002ULL, 7},
  { 0x4081020002000ULL, 0x801000208020200ULL, 5},
  { 0x8102040004000ULL, 0x6842030101010180ULL, 5},
  { 0x8040200020400ULL, 0x810108004041004ULL, 5},
  { 0x10080400040800ULL, 0xc2001500a2094ULL, 5},
  { 0x20100a000a1000ULL, 0x4400480810002240ULL, 7},
  { 0x40221400142200ULL, 0x4040000401080ULL, 9},
  { 0x2442800284400ULL, 0x301001041004000ULL, 9},
  { 0x4085000500800ULL, 0x4a0019011115ULL, 7},
  { 0x8102000201000ULL, 0x1082041638840140ULL, 5},
  { 0x10204000402000ULL, 0x280a008032084100ULL, 5},
  { 0x4020002040800ULL, 0x1304004480800ULL, 5},
  { 0x8040004081000ULL, 0x8013000090230ULL, 5},
  { 0x100a000a102000ULL, 0x404018808124202ULL, 7},
  { 0x22140014224000ULL, 0x220280800120a00ULL, 9},
  { 0x44280028440200ULL, 0x40010040660802ULL, 9},
  { 0x8500050080400ULL, 0x8100100082480ULL, 7},
  { 0x10200020100800ULL, 0x408011040490820ULL, 5},
  { 0x20400040201000ULL, 0x4018051248190840ULL, 5},
  { 0x2000204081000ULL, 0x4001441054804000ULL, 5},
  { 0x4000408102000ULL, 0x2030481844180800ULL, 5},
  { 0xa000a10204000ULL, 0x200164800040cULL, 7},
  { 0x14001422400000ULL, 0x10040a128010400ULL, 7},
  { 0x28002844020000ULL, 0x80104000040ULL, 7},
  { 0x50005008040200ULL, 0x25a0022048400200ULL, 7},
  { 0x20002010080400ULL, 0x4020018410820100ULL, 5},
  { 0x40004020100800ULL, 0x4085200482022ULL, 5},
  { 0x20408102000ULL, 0x88420221200104ULL, 5},
  { 0x40810204000ULL, 0x800404444202580ULL, 5},
  { 0xa1020400000ULL, 0x182002c654300100ULL, 5},
  { 0x142240000000ULL, 0x21c020880100ULL, 5},
  { 0x284402000000ULL, 0x420124c0002ULL, 5},
  { 0x500804020000ULL, 0x10042aa04430080ULL, 5},
  { 0x201008040200ULL, 0x1011040808005000ULL, 5},
  { 0x402010080400ULL, 0x208110c40820200ULL, 5},
  { 0x2040810204000ULL, 0x1010800820800ULL, 6},
  { 0x4081020400000ULL, 0x2008204414014880ULL, 5},
  { 0xa102040000000ULL, 0x420c000904011480ULL, 5},
  { 0x14224000000000ULL, 0x20084100840420ULL, 5},
  { 0x28440200000000ULL, 0x1c00040a20224402ULL, 5},
  { 0x50080402000000ULL, 0x125100821081220ULL, 5},
  { 0x20100804020000ULL, 0x800200801810c08ULL, 5},
  { 0x40201008040200ULL, 0x4298481046404100ULL, 6},
};