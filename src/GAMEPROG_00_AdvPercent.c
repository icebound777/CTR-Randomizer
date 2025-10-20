#include <common.h>

#include "saveslot_defines.h"

void GAMEPROG_AdvPercent(struct AdvProgress* adv)
{
    int i;
    int percent;
    int numGems;
    int bitIndex;
    struct GameTracker* gGT;
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));
    gGT = sdata->gGT;

    // start counter
    percent = 0;
    numGems = 0;

    // if beat oxide once, add 2% for first time (2-0=2%)
    // if beat oxide twice, add 1% for second time (2-1=1%)
    for (i = 0; i < 2; i++)
    {
        // first bit of beating oxide
        bitIndex = 0x73 + i;
        if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            percent += (2 - i);
        }
    }

    // assume all tracks have
    // gold or platinum relic
    percent += 1;

    // check all tracks just for relics
    for(i = 0; i < 18; i++)
    {
        // first bit of gold relic
        bitIndex = 0x28 + i;
        if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            // check next relic
            continue;
        }

        // if relic is not unlocked,
        // then extra 1% is not earned
        percent -= 1;

        // stop checking relics
        break;
    }

    // Find relic type with the highest number of relics acquired
    int sapphire_relics = (
        (advSlot2->SLOT2_NUM_RELICS_SAPPHIRE)
        + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE)
    );
    int gold_relics = (
        (advSlot2->SLOT2_NUM_RELICS_GOLD)
        + (advSlot3->SLOT2_NUM_RELICS_GOLD)
    );
    int platinum_relics = (
        (advSlot2->SLOT2_NUM_RELICS_PLATINUM)
        + (advSlot3->SLOT2_NUM_RELICS_PLATINUM)
    );
    int most_relics = sapphire_relics;
    if (most_relics < gold_relics) most_relics = gold_relics;
    if (most_relics < platinum_relics) most_relics = platinum_relics;

    percent += (
        (most_relics) * 2
        + (advSlot2->SLOT2_NUM_TROPHIES + advSlot3->SLOT2_NUM_TROPHIES) * 2
        + advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS
        + (advSlot2->SLOT2_NUM_TOKENS_RED) + (advSlot3->SLOT2_NUM_TOKENS_RED)
        + (advSlot2->SLOT2_NUM_TOKENS_GREEN) + (advSlot3->SLOT2_NUM_TOKENS_GREEN)
        + (advSlot2->SLOT2_NUM_TOKENS_BLUE) + (advSlot3->SLOT2_NUM_TOKENS_BLUE)
        + (advSlot2->SLOT2_NUM_TOKENS_YELLOW) + (advSlot3->SLOT2_NUM_TOKENS_YELLOW)
        + (advSlot2->SLOT2_NUM_TOKENS_PURPLE) + (advSlot3->SLOT2_NUM_TOKENS_PURPLE)
        + (advSlot2->SLOT2_NUM_GEMS) + (advSlot3->SLOT2_NUM_GEMS)
    );

    gGT->currAdvProfile.completionPercent = percent;
}
