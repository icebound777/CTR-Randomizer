#include <common.h>

/* START Randomizer */
#include "GAMEPROG_00_AdvPercent.h"
/* END Randomizer */

void GAMEPROG_NewProfile_InsideAdv(struct AdvProgress* adv)
{
    // clear
    memset(adv, 0x0, sizeof(struct AdvProgress));

    randomizer_set_profile_defaults(adv);
    /* END Randomizer */
}
