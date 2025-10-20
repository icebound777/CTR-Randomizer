#include <common.h>
/* START Randomizer */
#include "saveslot_defines.h"
/* END Randomizer */

void DECOMP_UI_DrawNumRelic(short posX, short posY)
{
    int num;
    struct GameTracker* gGT;
    char string[8];
    /* START Randomizer */
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));
    /* END Randomizer */

    DecalFont_DrawLine(&sdata->s_x[0], posX, posY+4, FONT_SMALL, ORANGE);

    gGT = sdata->gGT;
    num = ((advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE));
    if ((gGT->gameMode2 & 0x1000000) != 0) num--;

    sprintf(&string[0], &sdata->s_longInt[0], num);
    DecalFont_DrawLine(&string[0], posX+0xD, posY, FONT_BIG, ORANGE);
}
