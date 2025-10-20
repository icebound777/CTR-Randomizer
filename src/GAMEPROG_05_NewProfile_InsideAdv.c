#include <common.h>

/* START Randomizer */
#include "CTRRandomizer_outsourcing.h"
/* END Randomizer */

void GAMEPROG_NewProfile_InsideAdv(struct AdvProgress* adv)
{
    // clear
    memset(adv, 0x0, sizeof(struct AdvProgress));

    randomizer_set_profile_defaults(adv);
    /* END Randomizer */
}
