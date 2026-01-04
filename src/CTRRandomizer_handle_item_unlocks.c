#include <common.h>

#include "saveslot_defines.h"
#include "reward_enums.h"

void handle_item_unlocks(
    struct AdvProgress  *adv,
    int                 bitIndex,
    int                 item_type,
    int                 item_color
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
        switch (item_color)
        {
            case RELIC_SAPPHIRE:
                advSlot2->rewards[4] = (
                    (advSlot2->rewards[4] & 0xFFFFFF00)
                    | ((advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + 1)
                );
                break;
            case RELIC_GOLD:
                advSlot2->rewards[4] = (
                    (advSlot2->rewards[4] & 0xFFFF00FF)
                    | (((advSlot2->SLOT2_NUM_RELICS_GOLD) + 1) << 8)
                );
                break;
            case RELIC_PLATINUM:
                advSlot2->rewards[4] = (
                    (advSlot2->rewards[4] & 0xFF00FFFF)
                    | (((advSlot2->SLOT2_NUM_RELICS_PLATINUM) + 1) << 16)
                );
                break;
            default:
                ;
        }
    }
    else if (item_type == STATIC_KEY)
    {
        advSlot2->SLOT2_NUM_KEYS++;
    }
    else if (item_type == STATIC_TOKEN)
    {
        switch (item_color)
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
        advSlot2->rewards[5]++;
        advSlot2->rewards[5] |= (0x100 << item_color);
        //switch (item_color)
        //{
        //    case GEM_RED:
        //        advSlot2->rewards[5]++;
        //        advSlot2->rewards[5] |= 0x100;
        //        break;
        //    case GEM_GREEN:
        //        advSlot2->rewards[5]++;
        //        advSlot2->rewards[5] |= 0x200;
        //        break;
        //    case GEM_BLUE:
        //        advSlot2->rewards[5]++;
        //        advSlot2->rewards[5] |= 0x400;
        //        break;
        //    case GEM_YELLOW:
        //        advSlot2->rewards[5]++;
        //        advSlot2->rewards[5] |= 0x800;
        //        break;
        //    case GEM_PURPLE:
        //        advSlot2->rewards[5]++;
        //        advSlot2->rewards[5] |= 0x1000;
        //        break;
        //    default:
        //        ;
        //}
    }
}
