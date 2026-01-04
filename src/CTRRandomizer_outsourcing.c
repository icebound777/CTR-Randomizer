/*
This file houses code that would rather be in some other source files, but did
not fit there due to missing space for original functions to expand.
*/
#include <common.h>

#include "saveslot_defines.h"
#include "reward_enums.h"

struct Instance *advhud_relic = 0;
int pre_gemcup_levelid = 0;
short hud_relic_color_lock = 0;

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
    int num_relic_to_draw = 0;

    gGT = sdata->gGT;

    int relic_type = (gGT->timer / FPS_DOUBLE(0x3C)) % 3;

    // if we have a relic lock timer from the end of the podium, keep that relic
    // color and counter showing for longer, so the player can properly see the
    // count update.
    // gotta unpack the timer and the relic type from hud_relic_color_lock
    short relic_lock_timer = (hud_relic_color_lock & 0xFF0) >> 4;
    if (relic_lock_timer > 0)
    {
        relic_type = hud_relic_color_lock & 0xF;
        // repack hud_relic_color_lock
        hud_relic_color_lock = relic_type | ((relic_lock_timer - 1) << 4);
    }

    if ((gGT->gameMode2 & INC_RELIC) != 0) num_relic_to_draw--;

    if (relic_type == RELIC_SAPPHIRE)
    {
        advhud_relic->colorRGBA = 0x20a5ff0;
        num_relic_to_draw += ((advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE));
    }
    else if (relic_type == RELIC_GOLD)
    {
        advhud_relic->colorRGBA = 0xd8d2090;
        num_relic_to_draw += ((advSlot2->SLOT2_NUM_RELICS_GOLD) + (advSlot3->SLOT2_NUM_RELICS_GOLD));
    }
    else
    {
        advhud_relic->colorRGBA = 0xffede90;
        num_relic_to_draw += ((advSlot2->SLOT2_NUM_RELICS_PLATINUM) + (advSlot3->SLOT2_NUM_RELICS_PLATINUM));
    }

    return num_relic_to_draw;
}
