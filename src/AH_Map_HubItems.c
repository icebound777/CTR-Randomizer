#include <common.h>
/* START Randomizer */
#include "saveslot_defines.h"
#include "CTRRandomizer_database.h"
#include "VehBirth_0_TeleportSelf.h"
/* END Randomizer */

void AH_Map_HubItems(void* hubPtrs, short *param_2)
{
    struct GameTracker  *gGT;
    struct AdvProgress  *adv;

    short  levelID;
    short  hubID;
    short  sVar1;
    short  *trophies;
    bool   open;
    int    iVar3;
    u_int  bit;
    int    iVar5;
    int    uVar6;
    short  sVar7;
    short  sVar8;
    short  *psVar9;
    short  *psVar10;
    int    pos3D[3];
    int    local_40;
    int    local_3c;
    int    local_38;
    int    local_34;
    int    local_30;
    int    local_2c;

    /* START Randomizer */
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));
    /* END Randomizer */

    gGT = sdata->gGT;
    adv = &sdata->advProgress;
    levelID = gGT->levelID;

    psVar10 = D232.hubItemsXY_ptrArray[levelID - GEM_STONE_VALLEY];
    if (*psVar10 != -1)
    {
        psVar9 = psVar10 + 1;
        do
        {
            sVar8 = -1;
            sVar8 = -1;
            sVar7 = (short)0xffffffff; //???
            sVar7 = -1;
            sVar7 = -1;

            // iconType
            sVar1 = psVar9[2];

            open = true;

            // Arrow beach->gemstone
            if (sVar1 == -1)
            {
                sVar7 = 0;

                if (levelID == N_SANITY_BEACH)
                {
                    // locked if key < 1
                    sVar7 = ((advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS) < 1);
                }

                LAB_800b17e8:
                iVar5 = sVar7 << 0x10;
                sVar8 = sVar8;
                sVar7 = (short)sVar7;
                LAB_800b17ec:
                iVar5 = iVar5 >> 0x10;
            }
            else
            {
                if (-1 < sVar1)
                {
                    sVar7 = sVar7;

                    // gemstone valley
                    if (sVar1 == 4)
                    {
                        iVar3 = 0;
                        iVar5 = 0;

                        // check 4 boss keys
                        for (iVar3 = 0; iVar3 < 4; iVar3++)
                        {
                            bit = iVar3 + 0x5e;
                            if ((advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS) < 4) open = false;
                            #if 0 /* RANDOMIZER */
                            if (CHECK_ADV_BIT(adv->rewards, bit) == 0)
                            {
                                open = false;
                                break;
                            }
                            #endif
                        }

                        if (!open)
                        {
                            LAB_800b17e4:
                            sVar8 = 0;
                            goto LAB_800b17e8;
                        }
                        sVar7 = sdata->advProgress.rewards[3] & 4;
                    }
                    else // not gemstone valley
                    {
                        iVar5 = 0;

                        if (3 < sVar1)
                        {
                            iVar5 = -0x10000;
                            sVar8 = sVar8;

                            // saveLoad screen (0x64)
                            if (sVar1 == 100)
                            {
                                local_40 = (int) *psVar10 + -0x200;
                                local_3c = (int) *psVar9 + -0x100;

                                UI_Map_GetIconPos(hubPtrs, &local_40, &local_3c);

                                AH_Map_LoadSave_Full(
                                    local_40,
                                    local_3c,
                                    &D232.loadSave_pos[0],
                                    (char*) &D232.loadSave_col[0],
                                    0x800,
                                    (int) psVar9[1]
                                );

                                iVar5 = -0x10000;
                            }
                            goto LAB_800b17ec;
                        }

                        // did not use GOTO,
                        // must be == 3, for Boss Garage
                        int base = levelID - N_SANITY_BEACH;

                        unsigned short garage_requirement_logic = GARAGE_OPENING_VANILLA_WARPPADS; // default
                        int db_fetch_result = DB_VALUE_NOTFOUND;
                        unsigned short db_ret = database_fetch(
                            (DB_PREFIX_SETTINGS | SETTING_BOSS_GARAGE_OPENING) << 16,
                            &db_fetch_result
                        );
                        if (db_fetch_result == DB_VALUE_OK) garage_requirement_logic = db_ret;

                        if (garage_requirement_logic == GARAGE_OPENING_TROPHIES)
                        {
                            int required_trophies;
                            switch (levelID)
                            {
                                case N_SANITY_BEACH:
                                    required_trophies = 4;
                                    break;

                                case THE_LOST_RUINS:
                                    required_trophies = 8;
                                    break;

                                case GLACIER_PARK:
                                    required_trophies = 12;
                                    break;

                                default: // CITADEL_CITY
                                    required_trophies = 16;
                                    break;
                            }
                            if ((advSlot2->SLOT2_NUM_TROPHIES + advSlot3->SLOT2_NUM_TROPHIES) < required_trophies)
                            {
                                open = false;
                            }
                        }
                        else
                        {
                            trophies = &data.advHubTrackIDs[base * 4];
                            for (iVar3 = 0; iVar3 < 4; iVar3++)
                            {
                                int levelID_to_check = trophies[iVar3];

                                if (garage_requirement_logic == GARAGE_OPENING_CURRENTHUB_WARPPADS)
                                {
                                    // Adjust check to look at the warp pads in the current hub
                                    // compensating for any warp pad randomization
                                    unsigned short randomized_LevelID = database_fetch(
                                        (DB_PREFIX_LEVELIDS | levelID_to_check) << 16,
                                        &db_fetch_result
                                    );
                                    if (db_fetch_result == DB_VALUE_OK)
                                    {
                                        levelID_to_check = randomized_LevelID;
                                    }
                                }

                                short bitindex = randomizer_get_bitindex_for_garage_unlock(levelID_to_check);
                                if (CHECK_ADV_BIT(adv->rewards, bitindex) == 0)
                                {
                                    open = false;
                                    break;
                                }
                            }
                        }
                        if (!open) goto LAB_800b17e4;

                        // check if key is unlocked
                        sVar7 = CHECK_ADV_BIT(adv->rewards, (base + 0x5e));
                    }

                    // open, not beaten
                    sVar8 = 1;

                    iVar5 = -0x10000;

                    // boss is beaten
                    if (sVar7 != 0)
                    {
                        sVar8 = 2;
                    }
                    goto LAB_800b17ec;
                }

                // Arrow beach->glacier
                if (sVar1 == -4)
                {
                    // locked if keys < 2
                    sVar7 = ((advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS) < 2);
                    goto LAB_800b17e8;
                }
                if (sVar1 < -3)
                {
                    // Arrow glacier->citadel
                    if (sVar1 == -5)
                    {
                        // locked if keys < 3
                        sVar7 = ((advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS) < 3);
                        goto LAB_800b17e8;
                    }
                    iVar5 = -1;
                }
                else
                {
                    // either arrow on Gemstone hub,
                    // pointing to beach or to ruins
                    if ((sVar1 == -3) || (sVar1 == -2))
                    {
                        // never locked
                        sVar7 = 0;

                        goto LAB_800b17e8;
                    }
                    iVar5 = -1;
                }
            }

            if (-1 < iVar5)
            {
                local_38 = (int)*psVar10 + -0x200;
                local_34 = (int)*psVar9 + -0x100;
                UI_Map_GetIconPos(hubPtrs, &local_38, &local_34);
                if ((iVar5 == 0) && (D232.unkModeHubItems == 0))
                {
                    AH_Map_HubArrowOutter(
                        hubPtrs,
                        (int) *param_2,
                        local_38,
                        local_34,
                        (0x1000 - (u_short)psVar9[1]),
                        1
                    );
                    *param_2 = *param_2 + 1;
                }

                // if even frame
                if ((gGT->timer & FPS_DOUBLE(2)) == 0)
                {
                    iVar5 = (int) sVar7 * 6;
                }
                else
                {
                    iVar5 = ((int) sVar7 * 2 + 1) * 3;
                }

                AH_Map_HubArrow(
                    local_38,
                    local_34,
                    &D232.hubArrow_pos[0],
                    (char*) &D232.hubArrow_col1[iVar5],
                    0x800,
                    (int) psVar9[1]
                );
            }

            if (-1 < sVar8)
            {
                pos3D[0] = (int) *psVar10;
                pos3D[1] = 0;
                pos3D[2] = (int) *psVar9;

                // if beat boss race
                if (sVar8 == 2)
                {
                    // red
                    uVar6 = 3;
                }
                else
                {
                    // locked boss race
                    // sVar6 == 0

                    // grey
                    uVar6 = 0x17;

                    // open, not beaten
                    if (sVar8 == 1)
                    {
                        // blue and white
                        // depending on frames
                        uVar6 = 5;
                        if ((gGT->timer & FPS_DOUBLE(2)) != 0) uVar6 = 4;
                    }
                }

                // open, not beaten
                if (sVar8 == 1)
                {
                    D232.unkModeHubItems = sVar8;
                    local_30 = pos3D[0];
                    local_2c = pos3D[2];

                    UI_Map_GetIconPos(hubPtrs, &local_30, &local_2c);

                    AH_Map_HubArrowOutter(
                        hubPtrs,
                        (int) *param_2,
                        local_30,
                        local_2c,
                        0,
                        2
                    );

                    *param_2 = *param_2 + 1;
                }

                // draw star icon for boss
                UI_Map_DrawRawIcon(
                    (int) hubPtrs,
                    &pos3D[0],
                    0x37,
                    uVar6,
                    0,
                    0x1000
                );
            }
            psVar10 = psVar10 + 4;
            psVar9 = psVar9 + 4;
        } while (*psVar10 != -1);
    }
    return;
}
