#include <common.h>

#include "CTRRandomizer_database.h"

// rewards[3]
#define REWARD_MASKHINT_WELCOMETOARENA  0x400000
#define REWARD_MASKHINT_USINGWARPPAD    0x800000
#define REWARD_MASKHINT_ACCESSINGATRACK 0x1000000
#define REWARD_MASKHINT_BOSSGARAGE      0x2000000
#define REWARD_MASKHINT_ENTEROXIDESHIO  0x4000000
#define REWARD_MASKHINT_OPENDOORS       0x8000000
#define REWARD_MASKHINT_SAVELOADSCREEN  0x10000000
#define REWARD_MASKHINT_NEWWORLD        0x20000000

// rewards[4]
#define REWARD_MASKHINT_HANGTIMETURBO   0x10
#define REWARD_MASKHINT_POWERSLIDE      0x20
#define REWARD_MASKHINT_TURBOBOOST      0x40
#define REWARD_MASKHINT_BREAKSLIDE      0x80
#define REWARD_MASKHINT_NEEDTWOKEYS     0x100
#define REWARD_MASKHINT_WUMPAFRUIT      0x1000
#define REWARD_MASKHINT_TNT             0x2000
#define REWARD_MASKHINT_MAPINFO         0x4000
#define REWARD_MASKHINT_ARENACRYSTALS   0x8000
#define REWARD_MASKHINT_TOKENCHALLENGE  0x10000
#define REWARD_MASKHINT_GEMCUP          0x20000
#define REWARD_MASKHINT_NEED10RELICS    0x40000
#define REWARD_MASKHINT_RELICCHALLENGE  0x80000

void randomizer_set_profile_defaults(struct AdvProgress* adv)
{
    // no character selected
    adv->characterID = -1;

    // N Sane Beach
    adv->HubLevYouSavedOn = 0x1a;

    /* START Randomizer */
    int skip_mask_hints = false; // default
    int db_result = DB_VALUE_NOTFOUND;
    int db_ret = database_fetch(
        DB_PREFIX_SETTINGS | SETTING_QOL_SKIP_MASKHINTS,
        &db_result
    );
    if (db_result == DB_VALUE_OK) skip_mask_hints = db_ret;

    if (skip_mask_hints)
    {
        // Set all mask hints as already witnessed
        adv->rewards[3] = (
            REWARD_MASKHINT_WELCOMETOARENA  |
            REWARD_MASKHINT_USINGWARPPAD    |
            REWARD_MASKHINT_ACCESSINGATRACK |
            REWARD_MASKHINT_BOSSGARAGE      |
            REWARD_MASKHINT_ENTEROXIDESHIO  |
            REWARD_MASKHINT_OPENDOORS       |
            REWARD_MASKHINT_SAVELOADSCREEN  |
            REWARD_MASKHINT_NEWWORLD
        );
        adv->rewards[4] = (
            REWARD_MASKHINT_HANGTIMETURBO  |
            REWARD_MASKHINT_POWERSLIDE     |
            REWARD_MASKHINT_TURBOBOOST     |
            REWARD_MASKHINT_BREAKSLIDE     |
            REWARD_MASKHINT_NEEDTWOKEYS    |
            REWARD_MASKHINT_WUMPAFRUIT     |
            REWARD_MASKHINT_TNT            |
            REWARD_MASKHINT_MAPINFO        |
            REWARD_MASKHINT_ARENACRYSTALS  |
            REWARD_MASKHINT_TOKENCHALLENGE |
            REWARD_MASKHINT_GEMCUP         |
            REWARD_MASKHINT_NEED10RELICS   |
            REWARD_MASKHINT_RELICCHALLENGE
        );
    }
}
