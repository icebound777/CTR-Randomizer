#ifndef CTRRANDOMIZER_SAVESLOTDEFINES_H
#define CTRRANDOMIZER_SAVESLOTDEFINES_H

#define SLOT2_NUM_TROPHIES        rewards[0]
#define SLOT2_NUM_KEYS            rewards[1]
#define SLOT2_NUM_TOKENS_RED      rewards[2]       & 0xFF
#define SLOT2_NUM_TOKENS_GREEN    rewards[2] >> 8  & 0xFF
#define SLOT2_NUM_TOKENS_BLUE     rewards[2] >> 16 & 0xFF
#define SLOT2_NUM_TOKENS_YELLOW   rewards[2] >> 24 & 0xFF
#define SLOT2_NUM_TOKENS_PURPLE   rewards[3]       & 0xFF
#define SLOT2_NUM_RELICS_SAPPHIRE rewards[4]       & 0xFF
#define SLOT2_NUM_RELICS_GOLD     rewards[4] >> 8  & 0xFF
#define SLOT2_NUM_RELICS_PLATINUM rewards[4] >> 16 & 0xFF
#define SLOT2_NUM_GEMS            rewards[5] & 0xFF
#define SLOT2_CHECK_GEM_RED       rewards[5] & 0x100
#define SLOT2_CHECK_GEM_GREEN     rewards[5] & 0x200
#define SLOT2_CHECK_GEM_BLUE      rewards[5] & 0x400
#define SLOT2_CHECK_GEM_YELLOW    rewards[5] & 0x800
#define SLOT2_CHECK_GEM_PURPLE    rewards[5] & 0x1000

#endif