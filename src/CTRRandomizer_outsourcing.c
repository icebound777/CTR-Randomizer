/*
This file houses code that would rather be in some other source files, but did
not fit there due to missing space for original functions to expand.
*/
#include <common.h>

#include "CTRRandomizer_database.h"
#include "saveslot_defines.h"
#include "reward_enums.h"

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

struct Instance *advhud_relic = 0;

/*
Outsourced from GAMEPROG_05_NewProfile_InsideAdv.c
Sets initial character and hub values (moved from above file).
Then checks the chosen setting value for the mask hints skip qol, and
marks the hint flags if required.
*/
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

/*
Outsourced from AH_Garage_ThTick.c
Determines the boss ID for the current boss garage.
Adjusted by the randomizer checking the relics for Oxide differently.
*/
int randomizer_garage_tick_get_bossID(
    char  levelID,
    char  hubID
)
{
    int bossID;

    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));

    bossID = R232.bossIDs[hubID];

    if (levelID == GEM_STONE_VALLEY)
    {
        int oxide_relics_required = RELIC_SAPPHIRE; // default
        int db_result = DB_VALUE_OK;
        int db_ret = database_fetch(
            DB_PREFIX_SETTINGS | SETTING_OXIDE_REQUIRED_RELICS,
            &db_result
        );
        if (db_result == DB_VALUE_OK) oxide_relics_required = db_ret;

        // If Oxide needs Sapphires, then just take the number of Sapphires,
        // else add up Golds and Platinums and make sure they exceed 18
        int num_relics = (oxide_relics_required == RELIC_SAPPHIRE)
            ? (advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE)
            : (
                (advSlot2->SLOT2_NUM_RELICS_GOLD)
                + (advSlot3->SLOT2_NUM_RELICS_GOLD)
                + (advSlot2->SLOT2_NUM_RELICS_PLATINUM)
                + (advSlot3->SLOT2_NUM_RELICS_PLATINUM)
            )
        ;

        if (num_relics >= 18) bossID = 5;
    }

    return bossID;
}

/*
Outsourced from UI_34_DrawNumRelic.c
Calculate the number of relics to display on the adventure hud, and also cycle
the type of relic rendered.
*/
int randomizer_get_num_relic(void)
{
    struct GameTracker* gGT;
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));

    gGT = sdata->gGT;

    int relic_type = (gGT->timer / FPS_DOUBLE(0x3C)) % 3;

    if (relic_type == RELIC_SAPPHIRE)
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0x20a5ff0;
        return ((advSlot2->SLOT2_NUM_RELICS_SAPPHIRE) + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE));
    }
    else if (relic_type == RELIC_GOLD)
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0xd8d2090;
        return ((advSlot2->SLOT2_NUM_RELICS_GOLD) + (advSlot3->SLOT2_NUM_RELICS_GOLD));
    }
    else
    {
        if (advhud_relic != 0) advhud_relic->colorRGBA = 0xffede90;
        return ((advSlot2->SLOT2_NUM_RELICS_PLATINUM) + (advSlot3->SLOT2_NUM_RELICS_PLATINUM));
    }
}

/*
Outsourced from 223_RR_EndEvent_UnlockAward.c
Store relic time globally if reached Gold or Platinum
*/
void randomizer_store_relic_time(int time)
{
    sdata->relicTime_1min = time / 0xe100;
    sdata->relicTime_10sec = (time / 0x2580) % 6;
    sdata->relicTime_1sec = (time / 0x3c0) % 10;
    sdata->relicTime_1ms = ((time * 100) / 0x3c0) % 10;
    sdata->relicTime_10ms = 0;
}
