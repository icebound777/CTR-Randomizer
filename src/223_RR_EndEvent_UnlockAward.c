#include <common.h>
/* START RANDOMIZER */
#include "CTRRandomizer_handle_item_unlocks.h"
#include "CTRRandomizer_database.h"
#include "messages_handler.h"
#include "reward_enums.h"
#include "CTRRandomizer_outsourcing.h"

#include "UI_08_INSTANCE_InitAll.h"
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
    unsigned short db_ret;
    unsigned short require_perfect;

    driver = gGT->drivers[0];
    levelID = gGT->levelID;
    adv = &sdata->advProgress;

    raceTime = driver->timeElapsedInRace;
    all_time_crates = (driver->numTimeCrates == gGT->timeCratesInLEV);

    require_perfect = 0; // default: FALSE
    db_fetch_result = DB_VALUE_NOTFOUND;
    db_ret = database_fetch(
        (DB_PREFIX_SETTINGS | SETTING_RELIC_NEEDS_PERFECT) << 16,
        &db_fetch_result
    );
    if (db_fetch_result == DB_VALUE_OK) require_perfect = db_ret;

    // Only check relic times if we either don't require perfects, or we
    // did do a perfect run
    if (all_time_crates || !require_perfect)
    {
        unsigned short required_difficulty = RELICDIFF_SAPPHIRE; // default
        db_fetch_result = DB_VALUE_NOTFOUND;
        db_ret = database_fetch(
            (DB_PREFIX_SETTINGS | SETTING_RELIC_DIFFICULTY) << 16,
            &db_fetch_result
        );
        if (db_fetch_result == DB_VALUE_OK) required_difficulty = db_ret;

        // If you hit all time crates, subtract 10 seconds
        if (all_time_crates) raceTime -= 0x2580;

        // Did we even beat the required difficulty level
        relicTime = data.RelicTime[(levelID * 3) + required_difficulty];
        if (raceTime <= relicTime)
        {
            // Check the relic times for Sapphire, Gold, Platinum in order
            for (i = 0; i < 3; i++)
            {
                relicTime = data.RelicTime[(levelID * 3) + i];

                if (raceTime > relicTime) break; // Did not beat current time: abort

                bitIndex = 0x16 + (0x12 * i) + levelID;

                if (CHECK_ADV_BIT(adv->rewards, bitIndex) != 0) continue;

                gGT->gameModeEnd |= NEW_RELIC;

                unsigned short race_reward = STATIC_RELIC | (i << 16); // default
                db_fetch_result = DB_VALUE_NOTFOUND;
                db_ret = database_fetch(
                    (DB_PREFIX_REWARDS | levelID) << 16 | STATIC_RELIC | (i << 8),
                    &db_fetch_result
                );
                if (db_fetch_result == DB_VALUE_OK) race_reward = db_ret;
                handle_item_unlocks(
                    adv,
                    bitIndex,
                    GET_CLEAN_REWARD(race_reward),
                    GET_REWARD_COLOR(race_reward)
                );
                gGT->podiumRewardID = race_reward;

                if (i == 0)
                {
                    if (levelID == TURBO_TRACK)
                    {
                        // unlock turbo track
                        sdata->gameProgress.unlocks[0] |= 2;

                        enqueue_message(
                            MSGTYPE_UNLOCK,
                            "Turbo Track"
                        );
                    }
                    continue; // if Sapphire skip storing relic time
                }

                store_relic_time(relicTime);
            }
        }
    }
}
