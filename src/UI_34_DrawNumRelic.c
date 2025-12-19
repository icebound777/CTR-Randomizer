#include <common.h>
/* START Randomizer */
#include "CTRRandomizer_outsourcing.h"
/* END Randomizer */

void DECOMP_UI_DrawNumRelic(short posX, short posY)
{
    int num;
    struct GameTracker* gGT;
    char string[8];

    DecalFont_DrawLine(&sdata->s_x[0], posX, posY+4, FONT_SMALL, ORANGE);

    gGT = sdata->gGT;
    /* START Randomizer */
    num = randomizer_get_num_relic();
    /* END Randomizer */

    sprintf(&string[0], &sdata->s_longInt[0], num);
    DecalFont_DrawLine(&string[0], posX+0xD, posY, FONT_BIG, ORANGE);
}
