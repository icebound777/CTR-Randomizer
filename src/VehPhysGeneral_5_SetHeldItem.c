#include <common.h>

/* START RANDOMIZER */
#include "CTRRandomizer_database.h"
/* END RANDOMIZER */

enum ItemSet
{
    ITEMSET_Race1=0,
    ITEMSET_Race2,
    ITEMSET_Race3,
    ITEMSET_Race4,
    ITEMSET_BattleDefault,
    ITEMSET_BattleCustom,

    // these two swapped,
    // for the sake of array
    ITEMSET_BossRace,
    ITEMSET_CrystalChallenge
};

enum Items {
    ITEM_TURBO=0,
    ITEM_BOMB,
    ITEM_MISSILE,
    ITEM_TNT,
    ITEM_POTION,
    ITEM_UNUSED_FEATHER,
    ITEM_SHIELD,
    ITEM_MASK,
    ITEM_CLOCK,
    ITEM_WARPBALL,
    ITEM_BOMB_X3,
    ITEM_MISSILE_X3
};

// all except CrystalChallenge
extern char* charPtr[7];
extern char numWeapons[7];

// Itemset infographic (outdated):
// https://discord.com/channels/330945093416779787/550106151887568906/734368526294450267
void DECOMP_VehPhysGeneral_SetHeldItem(
    struct Driver *driver
)
{
    u_int rng;
    int itemSet;
    char item;
    char bossFails;
    struct GameTracker *gGT;
    u_int (*FUN_8003ea28)() = 0x8003ea28;

    gGT = sdata->gGT;

    // 6th Itemset (Battle Mode Custom Itemset)
    itemSet = ITEMSET_BattleCustom;

    // 5th Itemset (Battle Mode Default Itemset, 0x34de)
    if (gGT->battleSetup.enabledWeapons == 0x34de) itemSet = ITEMSET_BattleDefault;

    // Not in Battle Mode
    if ((gGT->gameMode1 & BATTLE_MODE) == 0)
    {
        // 7th Itemset (Crystal Challenge)
        itemSet = ITEMSET_CrystalChallenge;

        // Not in Crystal Challenge
        if ((gGT->gameMode1 & CRYSTAL_CHALLENGE) == 0)
        {
            // Choose Itemset based on number of Drivers
            int mode = gGT->numPlyrCurrGame + gGT->numBotsNextGame;

            switch (mode)
            {
                case 2: // if boss race
                    // boss race, last place
                    itemSet = ITEMSET_BossRace;

                    // if in first place
                    if (driver->driverRank == 0)
                    {
                        Itemset1:
                        // 1st Itemset
                        itemSet = ITEMSET_Race1;
                    }
                    break;

                case 3: // 3P VS race
                    // if first place
                    if (driver->driverRank == 0) goto Itemset1;

                    // default (2nd or 3rd place)
                    itemSet = ITEMSET_Race4;

                    // 50/50 chance of an upgrade,
                    // while in 2nd place
                    if (driver->driverRank == 1)
                    {
                        itemSet = ITEMSET_Race3;
                        rng = (*FUN_8003ea28)(); // DECOMP_MixRNG_Scramble();
                        if (rng & 1) goto Itemset2;
                    }
                    break;

                case 4:
                    itemSet = driver->driverRank;
                    break;

                case 5:
                    itemSet = driver->driverRank;
                    // 5th rank is 4th Itemset
                    if (itemSet == 4) itemSet = 3;
                    break;

                case 6: // 2P Arcade
                    // careful, dont get confused by names
                    itemSet = driver->driverRank;

                    // if 1st place, ItemSet1
                    if (itemSet == 0) goto Itemset1;

                    // if 6th place, ItemSet4
                    if (itemSet == 5)
                    {
                        itemSet = ITEMSET_Race4;
                    }
                    else {
                        // 2nd, 3rd place, gets 2nd Itemset
                        // 4th, 5th place, gets 3rd Itemset
                        itemSet = (itemSet - 1)/2 + 1;
                    }

                    break;

                case 8: // 1P Arcade & Adventure
                    // 0,1 = 0 (itemset1)
                    // 2,3 = 1 (itemset2)
                    // 4,5 = 2 (itemset3)
                    // 6,7 = 3 (itemset4)
                    itemSet = driver->driverRank >> 1;

                    // if in 2nd place, get itemSet2
                    if (itemSet == 1)
                    {
                        Itemset2:
                        itemSet = ITEMSET_Race2;
                    }
                    break;
            }
        }

        // if you have 4th-place itemset on first lap,
        // then override to 3rd place
        if (   itemSet == ITEMSET_Race4
            && driver->lapIndex == 0
        )
        {
            itemSet = ITEMSET_Race3;
        }
    }

    // Decide item for Driver
    rng = ((*FUN_8003ea28)() >> 0x3) % 0xc8; // DECOMP_MixRNG_Scramble()

    // number of weapons for RNG
    numWeapons[ITEMSET_BattleCustom] = gGT->battleSetup.numWeapons;

    /* START RANDOMIZER */
    driver->heldItemID = ITEM_TURBO;

    short db_prefix = 0;

    if (   gGT->levelID == PAPU_PYRAMID
        && itemSet != ITEMSET_BossRace
        && (driver->driverRank == 6 || driver->driverRank == 7)
    )
    {
        db_prefix = DB_PREFIX_SETTINGS | SETTING_HELPER_TIZIANO;
    }
    else if (   gGT->levelID == TINY_ARENA
             && (driver->driverRank == 0)
    )
    {
        db_prefix = DB_PREFIX_SETTINGS | SETTING_HELPER_TA;
    }

    if (db_prefix != 0)
    {
        int db_fetch_result = DB_VALUE_NOTFOUND;
        short db_ret = database_fetch(
            db_prefix << 16,
            &db_fetch_result
        );
        if (   db_fetch_result == DB_VALUE_OK
            && db_ret != 0
        )
        {
            driver->heldItemID = (gGT->levelID == TINY_ARENA)
                ? ITEM_TNT
                : ITEM_MASK
            ;
        }
    }

    if (driver->heldItemID == 0)
    /* END RANDOMIZER */
    switch (itemSet)
    {
        case ITEMSET_Race1:
        case ITEMSET_Race2:
        case ITEMSET_Race3:
        case ITEMSET_Race4:
        case ITEMSET_BattleDefault:
        case ITEMSET_BossRace:
            driver->heldItemID = charPtr[itemSet][(rng * numWeapons[itemSet]) / 0xc8];
            break;

        // uses int array instead of char,
        // should fix that later, requires 230 rewrite
        case ITEMSET_BattleCustom:
            driver->heldItemID = ((int*)charPtr[itemSet])[(rng * numWeapons[itemSet]) / 0xc8];
            break;

        case ITEMSET_CrystalChallenge:
            // Item is bomb at Rocky Road, Nitro Court
            // Item is turbo at Skull Rock and Rampage Ruins
            item = ITEM_BOMB;
            if (   gGT->levelID != SKULL_ROCK
                && gGT->levelID != RAMPAGE_RUINS
            )
            {
                goto SetItem;
            }
            driver->heldItemID = ITEM_TURBO;
            break;

        default: // "-1st place": Undecided rank
            rng = (*FUN_8003ea28)(); // DECOMP_MixRNG_Scramble();
            item = (char)rng + -0xc*((char)(rng / 6 + (rng >> 0x1f) >> 1) - (char)(rng >> 0x1f));
            SetItem:
            driver->heldItemID = item;
    }

    // In Boss race
    if (gGT->gameMode1 & ADVENTURE_BOSS)
    {
        bossFails = sdata->advProgress.timesLostBossRace[gGT->bossID];

        if (bossFails < 3)
        {
            // Replace Clock, Mask,  with 3 Missiles
            if ((u_int)driver->heldItemID - 0x7 < 0x3)
            {
                driver->heldItemID = ITEM_MISSILE_X3;
            }
        }
        else if (bossFails < 4)
        {
            // Replace Clock, Mask with 3 Missiles
            if ((u_int)driver->heldItemID - 0x7 < 0x2)
            {
                driver->heldItemID = ITEM_MISSILE_X3;
            }
        }
        else if (   bossFails < 0x5
                 && driver->heldItemID == ITEM_CLOCK
        )
        {
            // Replace Clock with 3 Missiles
            driver->heldItemID = ITEM_MISSILE_X3;
        }

        // Replace 3 Missiles with 1 Missile if racing Komodo Joe
        if (   gGT->levelID == DRAGON_MINES
            && driver->heldItemID == ITEM_MISSILE_X3
        )
        {
            driver->heldItemID = ITEM_MISSILE;
        }
    }

#if 0
    // === Removed ND Code ===
    // Spring is not in the RNG anyway

    // Replace unused Spring item with Turbo
    if (driver->heldItemID == ITEM_UNUSED_FEATHER)
        driver->heldItemID = 0x0;
#endif

    // Make sure only 1 Warpball is instanced at once
    if (driver->heldItemID == ITEM_WARPBALL)
    {
        // if nobody has warpball, then set flag that somebody has it
        if ((gGT->gameMode1 & WARPBALL_HELD) == 0)
        {
            gGT->gameMode1 |= WARPBALL_HELD;
        }
        else
        {
            // if somebody has warpball already, then give 3 missiles
            driver->heldItemID = ITEM_MISSILE_X3;
        }
    }

    if (   driver->heldItemID == ITEM_MISSILE_X3  // if you got 3 missiles
        && gGT->numPlyrCurrGame > 2 // if more than 2 players
        && ((gGT->gameMode1 & BATTLE_MODE) == 0) // if not in battle mode
    )
    {
        // if less than 2 drivers have 3 missiles, then increase number of drivers that have it
        if (gGT->numPlayersWith3Missiles < 2)
        {
            gGT->numPlayersWith3Missiles++;
        }
        else
        {
            // if 2 drivers already have 3 missiles, now you have 1 missile
            driver->heldItemID = ITEM_MISSILE;
        }
    }

    // Set number of held items
    if ((u_int)driver->heldItemID - 0xA < 0x2) driver->numHeldItems = 3;
}

char* charPtr[7] =
{
    &data.RNG_itemSetRace1[0],
    &data.RNG_itemSetRace2[0],
    &data.RNG_itemSetRace3[0],
    &data.RNG_itemSetRace4[0],
    &data.RNG_itemSetBattleDefault[0],
    (char *)&sdata_static.gameTracker.battleSetup.RNG_itemSetCustom[0],
    &data.RNG_itemSetBossrace[0]
};

char numWeapons[7] =
{
    0x14,
    0x34,
    0x14,
    0x13,
    0x14,
    -1,
    0x14
};
