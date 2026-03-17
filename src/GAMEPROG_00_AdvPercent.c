#include <common.h>

#include "saveslot_defines.h"
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

void GAMEPROG_AdvPercent(struct AdvProgress* adv)
{
    int i;
    int percent;
    int bitIndex;
    struct GameTracker *gGT;
    // If we are currently counting percent for curAdvProgress, use local items from slot 1 (+0x50)
    // But if we are currently counting percent for save slot 0, instead use local items
    // from slot 3 (+ another 0xA0)
    // If we are currently drawing curAdvProgress, use local items from slot 1 (+0x50)
    // But if we are currently drawing save slot 0, instead use local items from slot 3 (+ another 0xA0)
    struct AdvProgress *advSlotLocalItems = ((int) adv != (int) sdata->ptrToMemcardBuffer2 + 4)
        ? ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4)) // slot 1
        : ((struct AdvProgress*) (sdata->memcardBytes + 0xF0 + 4)) // slot 3
    ;
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));

    gGT = sdata->gGT;

    // start counter
    percent = 0;

    // if beat oxide once, add 2% for first time (2-0=2%)
    // if beat oxide twice, add 1% for second time (2-1=1%)
    for (i = 0; i < 2; i++)
    {
        // first bit of beating oxide
        bitIndex = 0x73 + i;
        if (CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            percent += (2 - i);
        }
    }

    // Find relic type with the highest number of relics acquired
    int sapphire_relics = (
        (advSlotLocalItems->SLOT2_NUM_RELICS_SAPPHIRE)
        + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE)
    );
    int gold_relics = (
        (advSlotLocalItems->SLOT2_NUM_RELICS_GOLD)
        + (advSlot3->SLOT2_NUM_RELICS_GOLD)
    );
    int platinum_relics = (
        (advSlotLocalItems->SLOT2_NUM_RELICS_PLATINUM)
        + (advSlot3->SLOT2_NUM_RELICS_PLATINUM)
    );
    int most_relics = sapphire_relics;
    if (most_relics < gold_relics) most_relics = gold_relics;
    if (most_relics < platinum_relics) most_relics = platinum_relics;

    // extra percent point for having 36+ total relics
    // this guarantees we have at least 18 (gold+plat) relics
    // in vanilla this was the "all golds or plats flags" check
    if (sapphire_relics + gold_relics + platinum_relics >= 36)
    {
        percent++;
    }

    percent += (
        (most_relics) * 2
        + (advSlotLocalItems->SLOT2_NUM_TROPHIES + advSlot3->SLOT2_NUM_TROPHIES) * 2
        + advSlotLocalItems->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS
        + (advSlotLocalItems->SLOT2_NUM_TOKENS_RED) + (advSlot3->SLOT2_NUM_TOKENS_RED)
        + (advSlotLocalItems->SLOT2_NUM_TOKENS_GREEN) + (advSlot3->SLOT2_NUM_TOKENS_GREEN)
        + (advSlotLocalItems->SLOT2_NUM_TOKENS_BLUE) + (advSlot3->SLOT2_NUM_TOKENS_BLUE)
        + (advSlotLocalItems->SLOT2_NUM_TOKENS_YELLOW) + (advSlot3->SLOT2_NUM_TOKENS_YELLOW)
        + (advSlotLocalItems->SLOT2_NUM_TOKENS_PURPLE) + (advSlot3->SLOT2_NUM_TOKENS_PURPLE)
        + (advSlotLocalItems->SLOT2_NUM_GEMS) + (advSlot3->SLOT2_NUM_GEMS)
    );

    gGT->currAdvProfile.completionPercent = percent;
}

void randomizer_set_profile_defaults(struct AdvProgress *adv)
{
    // no character selected
    adv->characterID = -1;

    // N Sane Beach
    adv->HubLevYouSavedOn = 0x1a;

    struct AdvProgress *memAdvProgressSlot1 = &((struct MemcardProfile *) sdata->ptrToMemcardBuffer2)->advProgress[1];
    memAdvProgressSlot1->rewards[0] = 0;
    memAdvProgressSlot1->rewards[1] = 0;
    memAdvProgressSlot1->rewards[2] = 0;
    memAdvProgressSlot1->rewards[3] = 0;
    memAdvProgressSlot1->rewards[4] = 0;
    memAdvProgressSlot1->rewards[5] = 0;

    /* START Randomizer */
    unsigned short skip_mask_hints = false; // default
    int db_result = DB_VALUE_NOTFOUND;
    unsigned short db_ret = database_fetch(
        (DB_PREFIX_SETTINGS | SETTING_QOL_SKIP_MASKHINTS) << 16,
        &db_result
    );
    if (db_result == DB_VALUE_OK) skip_mask_hints = db_ret;

    // Always skip new world hint: If we didn't, then the post-RipperRoo
    // cutscene not playing can crash the game.
    adv->rewards[3] = REWARD_MASKHINT_NEWWORLD;

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
