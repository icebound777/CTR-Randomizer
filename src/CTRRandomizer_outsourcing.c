/*
This file houses code that would rather be in some other source files, but did
not fit there due to missing space for original functions to expand.
*/
#include <common.h>

#include "saveslot_defines.h"
#include "reward_enums.h"

struct Instance *advhud_relic = 0;


/*
Outsourced from UI_34_DrawNumRelic.c
Calculate the number of relics to display on the adventure hud, and also cycle
the type of relic rendered.
*/
int randomizer_get_num_relic(void)
{
    struct GameTracker* gGT;
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));

    gGT = sdata->gGT;

    int relic_type = (gGT->timer / FPS_DOUBLE(0x3C)) % 3;

    if (relic_type == RELIC_SAPPHIRE)
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0x20a5ff0;
        return ((advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE));
    }
    else if (relic_type == RELIC_GOLD)
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0xd8d2090;
        return ((advSlot2->SLOT2_NUM_RELICS_GOLD) + (advSlot3->SLOT2_NUM_RELICS_GOLD));
    }
    else
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0xffede90;
        return ((advSlot2->SLOT2_NUM_RELICS_PLATINUM) + (advSlot3->SLOT2_NUM_RELICS_PLATINUM));
    }
}
