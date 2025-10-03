#include <common.h>

#include "saveslot_defines.h"

void handle_item_unlocks(
    struct AdvProgress  *adv,
    int                 bitIndex,
    int                 item_type
)
{
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));

    UNLOCK_ADV_BIT(adv->rewards, bitIndex);

    if (item_type == STATIC_TROPHY)
    {
        advSlot2->SLOT2_NUM_TROPHIES++;
    }
}
