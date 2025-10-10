#include <common.h>
/* START RANDOMIZER */
#include "CTRRandomizer_handle_item_unlocks.h"
#include "reward_enums.h"
/* END RANDOMIZER */

void RR_EndEvent_UnlockAward(void)
{
    struct GameTracker *gGT = sdata->gGT;
    struct Driver *driver;
    struct AdvProgress *adv;
    int levelID;
    int bitIndex;
    int raceTime;
    int relicTime;
    int i;

    driver = gGT->drivers[0];
    levelID = gGT->levelID;
    adv = &sdata->advProgress;

    raceTime = driver->timeElapsedInRace;

    // If you hit all time crates, subtract 10 seconds
    if (driver->numTimeCrates == gGT->timeCratesInLEV) raceTime -= 0x2580;

    // Check the relic times for Sapphire, Gold, Platinum in order
    for (i = 0; i < 3; i++)
    {
        relicTime = data.RelicTime[(levelID * 3) + i];

        if (raceTime > relicTime) break; // Did not beat current time: abort

        bitIndex = 0x16 + (0x12 * i) + levelID;

        if (CHECK_ADV_BIT(adv->rewards, bitIndex) != 0) continue;

        UNLOCK_ADV_BIT(adv->rewards, bitIndex);

        gGT->podiumRewardID = STATIC_RELIC;
        gGT->gameModeEnd |= NEW_RELIC;

        if (i == 0)
        {
            if (levelID == TURBO_TRACK)
            {
                // unlock turbo track
                sdata->gameProgress.unlocks[0] |= 2;
            }
            /* START RANDOMIZER */
            handle_item_unlocks(
                adv,
                -1,
                STATIC_RELIC,
                TOKEN_NONE
            );
            /* END RANDOMIZER */
            continue; // if Sapphire skip storing relic time
        }

        // store relic time globally if reached Gold or Platinum
        sdata->relicTime_1min = relicTime / 0xe100;
        sdata->relicTime_10sec = (relicTime / 0x2580) % 6;
        sdata->relicTime_1sec = (relicTime / 0x3c0) % 10;
        sdata->relicTime_1ms = ((relicTime * 100) / 0x3c0) % 10;
        sdata->relicTime_10ms = 0;
    }
}
