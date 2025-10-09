#include <common.h>

#include "saveslot_defines.h"

enum TokenAndGemColors {
    GEM_RED = 0,
    GEM_GREEN,
    GEM_BLUE,
    GEM_YELLOW,
    GEM_PURPLE,
    TOKEN_RED,
    TOKEN_GREEN,
    TOKEN_BLUE,
    TOKEN_YELLOW,
    TOKEN_PURPLE,
    TOKEN_NONE
};

void handle_item_unlocks(
    struct AdvProgress  *adv,
    int                 bitIndex,
    int                 item_type,
    int                 token_or_gem_type
)
{
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));

    if (bitIndex != -1) UNLOCK_ADV_BIT(adv->rewards, bitIndex);

    if (item_type == STATIC_TROPHY)
    {
        advSlot2->SLOT2_NUM_TROPHIES++;
    }
    else if (item_type == STATIC_RELIC)
    {
        advSlot2->SLOT2_NUM_RELICS++;
    }
    else if (item_type == STATIC_KEY)
    {
        advSlot2->SLOT2_NUM_KEYS++;
    }
    else if (item_type == STATIC_TOKEN)
    {
        switch (token_or_gem_type)
        {
            case TOKEN_RED:
                advSlot2->rewards[2] = (
                    (advSlot2->rewards[2] & 0xFFFFFF00)
                    | ((advSlot2->SLOT2_NUM_TOKENS_RED) + 1)
                );
                break;

            case TOKEN_GREEN:
                advSlot2->rewards[2] = (
                    (advSlot2->rewards[2] & 0xFFFF00FF)
                    | (((advSlot2->SLOT2_NUM_TOKENS_GREEN) + 1) << 8)
                );
                break;

            case TOKEN_BLUE:
                advSlot2->rewards[2] = (
                    (advSlot2->rewards[2] & 0xFF00FFFF)
                    | (((advSlot2->SLOT2_NUM_TOKENS_BLUE) + 1) << 16)
                );
                break;

            case TOKEN_YELLOW:
                advSlot2->rewards[2] = (
                    (advSlot2->rewards[2] & 0x00FFFFFF)
                    | (((advSlot2->SLOT2_NUM_TOKENS_YELLOW) + 1) << 24)
                );
                break;

            case TOKEN_PURPLE:
                advSlot2->rewards[3]++;
                break;

            default:
                ;
        }
    }
    else if (item_type == STATIC_GEM)
    {
        switch (token_or_gem_type)
        {
            case GEM_RED:
                advSlot2->rewards[5]++;
                advSlot2->rewards[5] |= 0x100;
                break;
            case GEM_GREEN:
                advSlot2->rewards[5]++;
                advSlot2->rewards[5] |= 0x200;
                break;
            case GEM_BLUE:
                advSlot2->rewards[5]++;
                advSlot2->rewards[5] |= 0x400;
                break;
            case GEM_YELLOW:
                advSlot2->rewards[5]++;
                advSlot2->rewards[5] |= 0x800;
                break;
            case GEM_PURPLE:
                advSlot2->rewards[5]++;
                advSlot2->rewards[5] |= 0x1000;
                break;
            default:
                ;
        }
    }
}
