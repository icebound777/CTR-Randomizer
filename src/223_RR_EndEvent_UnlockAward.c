#include <common.h>
/* START RANDOMIZER */
#include "CTRRandomizer_handle_item_unlocks.h"
#include "reward_enums.h"
#include "CTRRandomizer_database.h"
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
    bool all_time_crates;

    int db_fetch_result;
    int db_ret;
    int require_perfect;

    driver = gGT->drivers[0];
    levelID = gGT->levelID;
    adv = &sdata->advProgress;

    raceTime = driver->timeElapsedInRace;
    all_time_crates = (driver->numTimeCrates == gGT->timeCratesInLEV);

    require_perfect = 0; // default: FALSE
    db_fetch_result = DB_VALUE_NOTFOUND;
    db_ret = database_fetch(
        DB_PREFIX_SETTINGS | SETTING_RELIC_NEEDS_PERFECT,
        &db_fetch_result
    );
    if (db_fetch_result == DB_VALUE_OK) require_perfect = db_ret;

    // Only check relic times if we either don't require perfects, or we
    // did do a perfect run
    if (all_time_crates || !require_perfect)
    {

        int required_difficulty = RELICDIFF_SAPPHIRE; // default
        db_fetch_result = DB_VALUE_NOTFOUND;
        db_ret = database_fetch(
            DB_PREFIX_SETTINGS | SETTING_RELIC_DIFFICULTY,
            &db_fetch_result
        );
        if (db_fetch_result == DB_VALUE_OK) required_difficulty = db_ret;

        // If you hit all time crates, subtract 10 seconds
        if (all_time_crates) raceTime -= 0x2580;

        // Check the relic times for Sapphire, Gold, Platinum in order
        for (i = 0; i < 3; i++)
        {
            relicTime = data.RelicTime[(levelID * 3) + i];

            if (raceTime > relicTime) break; // Did not beat current time: abort

            bitIndex = 0x16 + (0x12 * i) + levelID;

            if (CHECK_ADV_BIT(adv->rewards, bitIndex) != 0) continue;

            UNLOCK_ADV_BIT(adv->rewards, bitIndex);

            if (i >= required_difficulty) gGT->gameModeEnd |= NEW_RELIC;

            if (i == required_difficulty)
            {
                int race_reward = STATIC_RELIC; // default
                db_fetch_result = DB_VALUE_NOTFOUND;
                db_ret = database_fetch(
                    DB_PREFIX_REWARDS | (levelID << 8) | STATIC_RELIC,
                    &db_fetch_result
                );
                if (db_fetch_result == DB_VALUE_OK) race_reward = db_ret;
                handle_item_unlocks(
                    adv,
                    -1,
                    GET_CLEAN_REWARD(race_reward),
                    GET_GEMANDTOKEN_COLOR(race_reward)
                );
                gGT->podiumRewardID = race_reward;
            }

            if (i == 0)
            {
                if (levelID == TURBO_TRACK)
                {
                    // unlock turbo track
                    sdata->gameProgress.unlocks[0] |= 2;
                }
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
}
