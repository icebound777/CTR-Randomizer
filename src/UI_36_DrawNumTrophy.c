#include <common.h>
/* START Randomizer */
#include "saveslot_defines.h"
/* END Randomizer */

void UI_DrawNumTrophy(int posX, int posY)
{
    int num;
    struct GameTracker* gGT;
    char string[8];
    /* START Randomizer */
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    /* END Randomizer */
    DecalFont_DrawLine(&sdata->s_x[0], posX, posY+4, FONT_SMALL, ORANGE);

    gGT = sdata->gGT;
    num = advSlot2->SLOT2_NUM_TROPHIES;
    if((gGT->gameMode2 & 0x4000000) != 0) num--;

    sprintf(&string[0], &sdata->s_longInt[0], num);
    DecalFont_DrawLine(&string[0], posX+0xD, posY, FONT_BIG, ORANGE);
}
