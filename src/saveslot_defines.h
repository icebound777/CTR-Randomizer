#ifndef CTRRANDOMIZER_SAVESLOTDEFINES_H
#define CTRRANDOMIZER_SAVESLOTDEFINES_H

#define SLOT2_NUM_TROPHIES       rewards[0]
#define SLOT2_NUM_KEYS           rewards[1]
#define SLOT2_NUM_TOKENS_RED     rewards[2]       & 0xFF
#define SLOT2_NUM_TOKENS_GREEN   rewards[2] >> 8  & 0xFF
#define SLOT2_NUM_TOKENS_BLUE    rewards[2] >> 16 & 0xFF
#define SLOT2_NUM_TOKENS_YELLOW  rewards[2] >> 24 & 0xFF
#define SLOT2_NUM_TOKENS_PURPLE  rewards[3]       & 0xFF
#define SLOT2_NUM_RELICS         rewards[4]
#define SLOT2_NUM_GEMS           rewards[5] & 0xFF

#endif