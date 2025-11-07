#include <common.h>
/* START Randomizer */
#include "CTRRandomizer_database.h"
#include "saveslot_defines.h"
#include "reward_enums.h"
/* END Randomizer */

void AH_WarpPad_ThTick(struct Thread *t);
void AH_WarpPad_ThDestroy(struct Thread *t);
void randomizer_set_instance_data(
    struct Instance* inst,
    unsigned short modelID,
    unsigned short modelColor
);

void AH_WarpPad_LInB(struct Instance* inst)
{
    int i;
    int levelID;
    struct Thread* t;
    struct WarpPad* warppadObj;

    struct GameTracker* gGT;

    int unlockItem_numOwned;
    unsigned short unlockItem_numNeeded;
    unsigned short unlockItem_modelID;
    unsigned short unlockItem_color;

    int* arrTokenCount;
    struct Instance* newInst;

    //for human reading purposes
    unsigned char ADV_CUP = 100;

    /* START Randomizer */
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));

    unsigned short db_ret;
    int db_fetch_result;
    /* END Randomizer */

    gGT = sdata->gGT;

    t =
        PROC_BirthWithObject
        (
            SIZE_RELATIVE_POOL_BUCKET
            (
                sizeof(struct WarpPad),
                NONE,
                MEDIUM,
                WARPPAD
            ),

            AH_WarpPad_ThTick,  // behavior
            0,                         // debug name
            0                          // thread relative
        );

    if(t == 0) return;
    inst->thread = t;
    t->inst = inst;

    t->funcThDestroy = AH_WarpPad_ThDestroy;

    // 0 - locked
    // 1 - open for trophy
    // 2 - unlocked all
    // 3 - open for relic/token
    // 4 - purple token or SlideCol/TurboTrack

    // locked
    t->modelIndex = 0;

    // make invisible
    // this is the red triangle
    // instance from DCxDemo's LEV Viewer
    inst->flags |= 0x80;

    warppadObj = t->object;
    warppadObj->levelID = 0; //this is dingo canyon
    warppadObj->boolEnteredWarppad = 0;
    warppadObj->framesWarping = 0;

    for(i = 0; i < WPIS_NUM_INSTANCES; i++)
        warppadObj->inst[i] = 0;

    // each warppad has a name "warppad#xxx"
    // "warppad#0" is dingo canyon, level ID 0
    // "warppad#16" is slide col, level ID 16
    // "warppad#102" is gem cup 2
    // "warppad#104" is gem cup 4
    // etc

    levelID = 0;
    for(i = 8; inst->name[i] != 0; i++)
    {
        levelID = levelID * 10 + inst->name[i] - '0';
    }

    /* START Randomizer */
    db_ret = database_fetch((DB_PREFIX_LEVELIDS | levelID) << 16, &db_fetch_result);
    if (db_fetch_result == DB_VALUE_OK) levelID = db_ret;
    /* END Randomizer */

    warppadObj->levelID = levelID;

    unlockItem_numNeeded = 0;
    unlockItem_numOwned = 0;
    unlockItem_modelID = STATIC_TROPHY;
    unlockItem_color = 0;

    if (   levelID < SLIDE_COLISEUM // Trophy Track
        && CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 6)) != 0 // trophy owned
    )
    {
        db_ret = database_fetch(
            (DB_PREFIX_WARPPADUNLOCK_2 | levelID) << 16,
            &db_fetch_result
        );
    }
    else // Trophy Track + Trophy owned, Turbo&Slide, Battle maps, Gem Cups
    {
        db_ret = database_fetch(
            (DB_PREFIX_WARPPADUNLOCK_1 | levelID) << 16,
            &db_fetch_result
        );
    }

    if (db_fetch_result == DB_VALUE_OK)
    {
        unlockItem_numNeeded = GET_REQUIREMENT_COUNT(db_ret);
        unlockItem_modelID = GET_REQUIREMENT_TYPE(db_ret);
        unlockItem_color = GET_REQUIREMENT_COLOR(db_ret);
    }

    switch (unlockItem_modelID)
    {
        case STATIC_TROPHY:
            unlockItem_numOwned = (advSlot2->SLOT2_NUM_TROPHIES + advSlot3->SLOT2_NUM_TROPHIES);
            break;

        case STATIC_KEY:
            unlockItem_numOwned = (advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS);
            break;

        case STATIC_RELIC:
            int sapphires_owned = (
                (advSlot2->SLOT2_NUM_RELICS_SAPPHIRE)
                + (advSlot3->SLOT2_NUM_RELICS_SAPPHIRE)
            );
            int golds_owned = (
                (advSlot2->SLOT2_NUM_RELICS_GOLD)
                + (advSlot3->SLOT2_NUM_RELICS_GOLD)
            );
            int platinums_owned = (
                (advSlot2->SLOT2_NUM_RELICS_PLATINUM)
                + (advSlot3->SLOT2_NUM_RELICS_PLATINUM)
            );
            switch (unlockItem_color)
            {
                case RELIC_SAPPHIRE:
                    unlockItem_numOwned = sapphires_owned;
                    break;

                case RELIC_GOLD:
                    unlockItem_numOwned = golds_owned;
                    break;

                case RELIC_PLATINUM:
                    unlockItem_numOwned = platinums_owned;
                    break;

                default: // case RELIC_ANY:
                    unlockItem_numOwned = (
                        sapphires_owned
                        + golds_owned
                        + platinums_owned
                    );
                    break;
            }
            break;

        case STATIC_TOKEN:
            int redtokens_owned = (
                (advSlot2->SLOT2_NUM_TOKENS_RED)
                + (advSlot3->SLOT2_NUM_TOKENS_RED)
            );
            int greentokens_owned = (
                (advSlot2->SLOT2_NUM_TOKENS_GREEN)
                + (advSlot3->SLOT2_NUM_TOKENS_GREEN)
            );
            int bluetokens_owned = (
                (advSlot2->SLOT2_NUM_TOKENS_BLUE)
                + (advSlot3->SLOT2_NUM_TOKENS_BLUE)
            );
            int yellowtokens_owned = (
                (advSlot2->SLOT2_NUM_TOKENS_YELLOW)
                + (advSlot3->SLOT2_NUM_TOKENS_YELLOW)
            );
            int purpletokens_owned = (
                (advSlot2->SLOT2_NUM_TOKENS_PURPLE)
                + (advSlot3->SLOT2_NUM_TOKENS_PURPLE)
            );
            switch (unlockItem_color)
            {
                case TOKEN_RED:
                    unlockItem_numOwned = redtokens_owned;
                    break;

                case TOKEN_GREEN:
                    unlockItem_numOwned = greentokens_owned;
                    break;

                case TOKEN_BLUE:
                    unlockItem_numOwned = bluetokens_owned;
                    break;

                case TOKEN_YELLOW:
                    unlockItem_numOwned = yellowtokens_owned;
                    break;

                case TOKEN_PURPLE:
                    unlockItem_numOwned = purpletokens_owned;
                    break;

                default: //case TOKEN_ANY:
                    unlockItem_numOwned = (
                        redtokens_owned
                        + greentokens_owned
                        + bluetokens_owned
                        + yellowtokens_owned
                        + purpletokens_owned
                    );
                    break;
            }
            break;

        case STATIC_GEM:
            switch (unlockItem_color)
            {
                case GEM_ANY:
                    unlockItem_numOwned = (
                        (advSlot2->SLOT2_NUM_GEMS)
                        + (advSlot3->SLOT2_NUM_GEMS)
                    );
                    break;
                case GEM_RED:
                    unlockItem_numOwned = (
                        ((advSlot2->SLOT2_CHECK_GEM_RED) + (advSlot3->SLOT2_CHECK_GEM_RED))
                        > 0
                    );
                    break;
                case GEM_GREEN:
                    unlockItem_numOwned = (
                        ((advSlot2->SLOT2_CHECK_GEM_GREEN) + (advSlot3->SLOT2_CHECK_GEM_GREEN))
                        > 0
                    );
                    break;
                case GEM_BLUE:
                    unlockItem_numOwned = (
                        ((advSlot2->SLOT2_CHECK_GEM_BLUE) + (advSlot3->SLOT2_CHECK_GEM_BLUE))
                        > 0
                    );
                    break;
                case GEM_YELLOW:
                    unlockItem_numOwned = (
                        ((advSlot2->SLOT2_CHECK_GEM_YELLOW) + (advSlot3->SLOT2_CHECK_GEM_YELLOW))
                        > 0
                    );
                    break;
                default: //case GEM_PURPLE:
                    unlockItem_numOwned = (
                        ((advSlot2->SLOT2_CHECK_GEM_PURPLE) + (advSlot3->SLOT2_CHECK_GEM_PURPLE))
                        > 0
                    );
                    break;
            }
            break;
    }

    // if unlocked
    if (unlockItem_numOwned >= unlockItem_numNeeded)
    {
        warppadObj->digit1s = 0;

        // if beam model exists
        if(gGT->modelPtr[STATIC_BEAM] != 0)
        {
            newInst = INSTANCE_Birth3D(gGT->modelPtr[STATIC_BEAM], 0, t);

            // copy matrix
            *(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
            *(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
            *(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
            *(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
            *(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
            newInst->matrix.t[0] = inst->matrix.t[0];
            newInst->matrix.t[1] = inst->matrix.t[1];
            newInst->matrix.t[2] = inst->matrix.t[2];

            newInst->alphaScale = 0xc00;

            warppadObj->inst[WPIS_OPEN_BEAM] = newInst;
        }

        // if spiral ring exists
        if(gGT->modelPtr[STATIC_BOTTOMRING] != 0)
        {
            for(i = 0; i < 2; i++)
            {
                newInst = INSTANCE_Birth3D(gGT->modelPtr[STATIC_BOTTOMRING], 0, t);

                // copy matrix
                *(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
                *(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
                *(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
                *(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
                *(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
                newInst->matrix.t[0] = inst->matrix.t[0];
                newInst->matrix.t[1] = inst->matrix.t[1] + i * 0x400;
                newInst->matrix.t[2] = inst->matrix.t[2];

                newInst->alphaScale = 0x400;

                warppadObj->inst[WPIS_OPEN_RING1+i] = newInst;
            }
        }

        for (i = 0; i < 3; i++) warppadObj->thirds[i] = 0x555*i;

        warppadObj->spinRot_Prize[0] = 0;
        warppadObj->spinRot_Prize[1] = 0;
        warppadObj->spinRot_Prize[2] = 0;

        warppadObj->spinRot_Beam[0] = 0;
        warppadObj->spinRot_Beam[1] = 0;
        warppadObj->spinRot_Beam[2] = 0;

        for (i = 0; i < 2; i++)
        {
            warppadObj->spinRot_Wisp[i][0] = 0;
            warppadObj->spinRot_Wisp[i][1] = 0;
            warppadObj->spinRot_Wisp[i][2] = 0;
        }

        if (levelID < SLIDE_COLISEUM)
        {
            // unlocked all
            t->modelIndex = 2;

            // if trophy not owned
            if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 6)) == 0)
            {
                // open for trophy
                t->modelIndex = 1;

                short reward = STATIC_TROPHY;
                short reward_color = 0;
                db_ret = database_fetch(
                    ((DB_PREFIX_REWARDS | levelID) << 16) | STATIC_TROPHY,
                    &db_fetch_result
                );
                if (db_fetch_result == DB_VALUE_OK)
                {
                    reward = GET_CLEAN_REWARD(db_ret);
                    reward_color = GET_REWARD_COLOR(db_ret);
                }

                newInst = INSTANCE_Birth3D(
                    gGT->modelPtr[reward],
                    0,
                    t
                );

                randomizer_set_instance_data(newInst, reward, reward_color);

                warppadObj->inst[WPIS_OPEN_PRIZE1] = newInst;
            }

            // if token not owned
            if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x4c)) == 0)
            {
                // not open for trophy
                if (t->modelIndex != 1)
                {
                    // open for relic/token
                    t->modelIndex = 3;
                }
BattleTrack:

                short reward = STATIC_TOKEN;
                short reward_color = data.metaDataLEV[levelID].ctrTokenGroupID;
                db_ret = database_fetch(
                    ((DB_PREFIX_REWARDS | levelID) << 16) | STATIC_TOKEN,
                    &db_fetch_result
                );
                if (db_fetch_result == DB_VALUE_OK)
                {
                    reward = GET_CLEAN_REWARD(db_ret);
                    reward_color = GET_REWARD_COLOR(db_ret);
                }

                newInst = INSTANCE_Birth3D(
                    gGT->modelPtr[reward],
                    0,
                    t
                );

                randomizer_set_instance_data(newInst, reward, reward_color);

                // === Naughty Dog Bug ===
                // They made an array where every token color
                // could have it's own specular light, but they're
                // all the same specLight, so just use the first one

                // And because they're all identical, dont bother storing
                // it, just give a pointer to the global, instead of a
                // pointer to warppadObj->specLight (ThTick and SpinRewards)
                #if 0
                warppadObj->specLightToken[0] = D232.specLightToken[0];
                warppadObj->specLightToken[1] = D232.specLightToken[1];
                warppadObj->specLightToken[2] = D232.specLightToken[2];
                #endif

                warppadObj->inst[WPIS_OPEN_PRIZE2] = newInst;
            }

SlideColTurboTrack:

            // if relic not owned
            if (levelID < NITRO_COURT) // check this cause of "goto BattleTrack"
            if (   CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x16)) == 0 // Sapphire
                || CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x2C)) == 0 // Gold
                || CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x42)) == 0 // Platinum
            )
            {
                // SlideCol/TurboTrack
                if (levelID >= SLIDE_COLISEUM)
                {
                    t->modelIndex = 4;
                }
                else if (t->modelIndex != 1)
                {
                    // open for token/relic
                    t->modelIndex = 3;
                }

                /* Set up not only the vanilla sapphire relic on instance slot
                   WPIS_OPEN_PRIZE3, but also the gold relic on unused instance
                   slot WPIS_CLOSED_ITEM and the platinum relic on unused
                   instance slot WPIS_CLOSED_X.
                   These also need special handling in WarpPad_ThTick.
                */
                short reward = STATIC_RELIC;
                short reward_color;
                short inst_index;
                for (short i = 0; i < 3; i++)
                {
                    reward_color = i; // SAPPHIRE, GOLD, PLATINUM
                    db_ret = database_fetch(
                        ((DB_PREFIX_REWARDS | levelID) << 16) | STATIC_RELIC | (reward_color << 8),
                        &db_fetch_result
                    );
                    if (db_fetch_result == DB_VALUE_OK)
                    {
                        reward = GET_CLEAN_REWARD(db_ret);
                        reward_color = GET_REWARD_COLOR(db_ret);
                    }

                    newInst = INSTANCE_Birth3D(
                        gGT->modelPtr[reward],
                        0,
                        t
                    );

                    randomizer_set_instance_data(newInst, reward, reward_color);

                    inst_index = (i == 0)
                        ? WPIS_OPEN_PRIZE3
                        : (i == 1)
                            ? WPIS_CLOSED_ITEM
                            : WPIS_CLOSED_X
                    ;
                    warppadObj->inst[inst_index] = newInst;
                }
            }

            for (i = 0; i < 5; i++)
            {
                /* Special handling for the new gold relic reward on
                   instance slot WPIS_CLOSED_ITEM and the new platinum relic
                   reward on instance slot WPIS_CLOSED_X
                */
                newInst = (i < 3)
                    ? warppadObj->inst[WPIS_OPEN_PRIZE1 + i]
                    : (i == 3)
                        ? warppadObj->inst[WPIS_CLOSED_ITEM]
                        : warppadObj->inst[WPIS_CLOSED_X]
                ;

                if (newInst == 0) continue;

                // copy matrix
                *(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
                *(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
                *(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
                *(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
                *(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
                newInst->matrix.t[0] = inst->matrix.t[0];
                newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
                newInst->matrix.t[2] = inst->matrix.t[2];
            }
        }
        else if(levelID < NITRO_COURT) // slide col, turbo track
        {
            // already unlocked
            t->modelIndex = 2;

            goto SlideColTurboTrack;
        }
        else if (levelID < GEM_STONE_VALLEY) // battle tracks
        {
            i = R232.battleTrackArr[levelID - NITRO_COURT] + 0x6f;

            // already unlocked
            t->modelIndex = 2;

            if (CHECK_ADV_BIT(sdata->advProgress.rewards, i) == 0)
            {
                // rainbow
                t->modelIndex = 4;

                goto BattleTrack;
            }
        }
        else // gemstone valley
        {
            // bit index of gem
            i = (levelID - ADV_CUP) + 0x6a;

            // if gem is already unlocked, quit
            if (CHECK_ADV_BIT(sdata->advProgress.rewards, i) != 0)
            {
                // beaten
                t->modelIndex = 2;

                return;
            }

            // rainbow color
            t->modelIndex = 4;

            short reward = STATIC_GEM;
            short reward_color = levelID - ADV_CUP;
            db_ret = database_fetch(
                ((DB_PREFIX_REWARDS | levelID) << 16) | STATIC_GEM,
                &db_fetch_result
            );
                if (db_fetch_result == DB_VALUE_OK)
                {
                    reward = GET_CLEAN_REWARD(db_ret);
                    reward_color = GET_REWARD_COLOR(db_ret);
                }

            newInst = INSTANCE_Birth3D(
                gGT->modelPtr[reward],
                0,
                t
            );

            randomizer_set_instance_data(newInst, reward, reward_color);

            warppadObj->inst[WPIS_OPEN_PRIZE1] = newInst;

            #if 0
            // store in Gem array
            warppadObj->specLightGem[0] = D232.specLightGem[0];
            warppadObj->specLightGem[1] = D232.specLightGem[1];
            warppadObj->specLightGem[2] = D232.specLightGem[2];
            #endif

            // for matrix copy
            goto SlideColTurboTrack;
        }

        return;
    }

    // === if locked ===

    if (unlockItem_numNeeded < 10)
    {
        warppadObj->digit10s = 0;
        warppadObj->digit1s = unlockItem_numNeeded;
    }
    else
    {
        warppadObj->digit10s = 1;
        warppadObj->digit1s = unlockItem_numNeeded - 10;
    }

    // ====== Item ========

    // WPIS_CLOSED_ITEM
    newInst = INSTANCE_Birth3D(gGT->modelPtr[unlockItem_modelID], 0, t);

    // copy matrix
    *(int*)((int)&newInst->matrix + 0x0) = *(int*)((int)&inst->matrix + 0x0);
    *(int*)((int)&newInst->matrix + 0x4) = *(int*)((int)&inst->matrix + 0x4);
    *(int*)((int)&newInst->matrix + 0x8) = *(int*)((int)&inst->matrix + 0x8);
    *(int*)((int)&newInst->matrix + 0xC) = *(int*)((int)&inst->matrix + 0xC);
    *(short*)((int)&newInst->matrix + 0x10) = *(short*)((int)&inst->matrix + 0x10);
    newInst->matrix.t[0] = inst->matrix.t[0];
    newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
    newInst->matrix.t[2] = inst->matrix.t[2];

    newInst->scale[0] = 0x2000;
    newInst->scale[1] = 0x2000;
    newInst->scale[2] = 0x2000;

    // no specular for trophy
    if (unlockItem_modelID != STATIC_TROPHY)
    {
        // specular lighting
        newInst->flags |= 0x20000;

        // relic
        if (unlockItem_modelID == STATIC_RELIC)
        {
            // Relic blue color
            if (unlockItem_color != RELIC_ANY)
            {
                newInst->colorRGBA = (unlockItem_color == RELIC_SAPPHIRE)
                    ? 0x20a5ff0
                    : (unlockItem_color == RELIC_GOLD)
                        ? 0xd8d2090
                        : 0xffede90
                ;
            }

            warppadObj->specLightRelic[0] = D232.specLightRelic[0];
            warppadObj->specLightRelic[1] = D232.specLightRelic[1];
            warppadObj->specLightRelic[2] = D232.specLightRelic[2];
        }
        else if (unlockItem_modelID == STATIC_KEY)
        {
            // Key color
            newInst->colorRGBA = 0xdca6000;

            // store in Gem array (intended by ND, not a bug)
            warppadObj->specLightGem[0] = D232.specLightGem[0];
            warppadObj->specLightGem[1] = D232.specLightGem[1];
            warppadObj->specLightGem[2] = D232.specLightGem[2];
        }
        else if (unlockItem_modelID == STATIC_GEM)
        {
            if (unlockItem_color != GEM_ANY)
            {
                // set color
                newInst->colorRGBA =
                    ((unsigned int)data.AdvCups[unlockItem_color].color[0] << 0x14) |
                    ((unsigned int)data.AdvCups[unlockItem_color].color[1] << 0xc) |
                    ((unsigned int)data.AdvCups[unlockItem_color].color[2] << 0x4);
            }
            // store in Gem array
            warppadObj->specLightGem[0] = D232.specLightGem[0];
            warppadObj->specLightGem[1] = D232.specLightGem[1];
            warppadObj->specLightGem[2] = D232.specLightGem[2];
        }
        else // assume token
        {
            // token color
            if (unlockItem_color != TOKEN_ANY)
            {
                newInst->colorRGBA =
                    ((unsigned int)data.AdvCups[unlockItem_color - 1].color[0] << 0x14) |
                    ((unsigned int)data.AdvCups[unlockItem_color - 1].color[1] << 0xc) |
                    ((unsigned int)data.AdvCups[unlockItem_color - 1].color[2] << 0x4);
            }

            // === Naughty Dog Bug ===
            // They made an array where every token color
            // could have it's own specular light, but they're
            // all the same specLight, so just use the first one

            warppadObj->specLightToken[0] = D232.specLightToken[0];
            warppadObj->specLightToken[1] = D232.specLightToken[1];
            warppadObj->specLightToken[2] = D232.specLightToken[2];
        }
    }

    newInst->flags |= (unlockItem_color << 20);
    warppadObj->inst[WPIS_CLOSED_ITEM] = newInst;

    // ====== "X" ========

    // WPIS_CLOSED_X
    newInst = INSTANCE_Birth3D(gGT->modelPtr[STATIC_BIGX], 0, t);

    // copy matrix
    *(int*)((int)&newInst->matrix + 0x0) = 0x1000;
    *(int*)((int)&newInst->matrix + 0x4) = 0;
    *(int*)((int)&newInst->matrix + 0x8) = 0x1000;
    *(int*)((int)&newInst->matrix + 0xC) = 0;
    *(short*)((int)&newInst->matrix + 0x10) = 0x1000;
    newInst->matrix.t[0] = inst->matrix.t[0];
    newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
    newInst->matrix.t[2] = inst->matrix.t[2];

    newInst->scale[0] = 0x2000;
    newInst->scale[1] = 0x2000;
    newInst->scale[2] = 0x2000;

    // always face camera
    newInst->model->headers[0].flags |= 1;

    warppadObj->inst[WPIS_CLOSED_X] = newInst;

    // ====== "10s" ========

    if(warppadObj->digit10s != 0)
    {
        // WPIS_CLOSED_10S
        newInst = INSTANCE_Birth3D(gGT->modelPtr[STATIC_BIG1], 0, t);

        // copy matrix
        *(int*)((int)&newInst->matrix + 0x0) = 0x1000;
        *(int*)((int)&newInst->matrix + 0x4) = 0;
        *(int*)((int)&newInst->matrix + 0x8) = 0x1000;
        *(int*)((int)&newInst->matrix + 0xC) = 0;
        *(short*)((int)&newInst->matrix + 0x10) = 0x1000;
        newInst->matrix.t[0] = inst->matrix.t[0];
        newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
        newInst->matrix.t[2] = inst->matrix.t[2];

        newInst->scale[0] = 0x2000;
        newInst->scale[1] = 0x2000;
        newInst->scale[2] = 0x2000;

        // always face camera
        for(i = 0; i < newInst->model->numHeaders; i++)
            newInst->model->headers[i].flags |= 1;

        warppadObj->inst[WPIS_CLOSED_10S] = newInst;
    }

    // ====== "1s" ========

    // STATIC_BIG (1-8)
    i = 0x38;
    if(warppadObj->digit1s == 0) i = 0x6d; // '0'
    if(warppadObj->digit1s == 9) i = 0x6e; // '9'

    // WPIS_CLOSED_1S
    newInst = INSTANCE_Birth3D(gGT->modelPtr[i], 0, t);

    // copy matrix
    *(int*)((int)&newInst->matrix + 0x0) = 0x1000;
    *(int*)((int)&newInst->matrix + 0x4) = 0;
    *(int*)((int)&newInst->matrix + 0x8) = 0x1000;
    *(int*)((int)&newInst->matrix + 0xC) = 0;
    *(short*)((int)&newInst->matrix + 0x10) = 0x1000;
    newInst->matrix.t[0] = inst->matrix.t[0];
    newInst->matrix.t[1] = inst->matrix.t[1] + 0x100;
    newInst->matrix.t[2] = inst->matrix.t[2];

    newInst->scale[0] = 0x2000;
    newInst->scale[1] = 0x2000;
    newInst->scale[2] = 0x2000;

    // always face camera
    for(i = 0; i < newInst->model->numHeaders; i++)
        newInst->model->headers[i].flags |= 1;

    warppadObj->inst[WPIS_CLOSED_1S] = newInst;
}

void randomizer_set_instance_data(
    struct Instance* inst,
    unsigned short modelID,
    unsigned short modelColor
)
{
    // Setting the scale in this module is literally useless, because it gets
    // overwritten in WardPad ThTick every frame anyway
    inst->scale[0] = 1;
    inst->scale[1] = 1;
    inst->scale[2] = 1;
    switch (modelID)
    {
        case STATIC_TROPHY:
            //inst->scale[0] = 0x2800;
            //inst->scale[1] = 0x2800;
            //inst->scale[2] = 0x2800;
            break;

        case STATIC_TOKEN:
            inst->flags |= 0x30000; // specular lighting
           // inst->scale[0] = 0x2000;
           // inst->scale[1] = 0x2000;
           // inst->scale[2] = 0x2000;
            inst->colorRGBA = (
                ((unsigned int)data.AdvCups[modelColor - 1].color[0] << 0x14)
                | ((unsigned int)data.AdvCups[modelColor - 1].color[1] << 0xc)
                | ((unsigned int)data.AdvCups[modelColor - 1].color[2] << 0x4)
            );
            break;

        case STATIC_RELIC:
            inst->flags |= 0x20000; // specular lighting
          //  inst->scale[0] = 0x1800;
          //  inst->scale[1] = 0x1800;
          //  inst->scale[2] = 0x1800;
            inst->colorRGBA = (modelColor == RELIC_SAPPHIRE)
                ? 0x20a5ff0 // relic blue
                : (modelColor == RELIC_GOLD)
                    ? 0xd8d2090 // relic gold
                    : 0xffede90 // relic platinum
            ;
            break;

        case STATIC_GEM:
            inst->flags |= 0x20000;
            if (modelColor < 5)
            {
                inst->colorRGBA = (
                    ((unsigned int)data.AdvCups[modelColor].color[0] << 0x14)
                    | ((unsigned int)data.AdvCups[modelColor].color[1] << 0xc)
                    | ((unsigned int)data.AdvCups[modelColor].color[2] << 0x4)
                );
            }
            else
            {
                // Gem with prizeColor == 5: our tag for multiworld-item
                // Make gem bright white
                inst->colorRGBA = 0xe0e0e00;
            }
            break;

        case STATIC_KEY:
         //   inst->scale[0] = 0x1000;
         //   inst->scale[1] = 0x1000;
         //   inst->scale[2] = 0x1000;
            inst->colorRGBA = 0xdca6000;
            break;
    }
}
