#include <common.h>

#include "CTRRandomizer_database.h"
#include "CTRRandomizer_outsourcing.h"
#include "reward_enums.h"

void DECOMP_AH_WarpPad_ThTick(struct Thread* t)
{
    int i;
    int j;
    int boolOpen;
    struct GameTracker *gGT;
    struct WarpPad *warppadObj;
    struct Instance *warppadInst;
    struct Instance **visInstSrc;
    struct Instance **instArr;

    struct Driver *driver;
    struct Instance *driverInst;

    int modelID;
    int levelID;
    int x;
    int y;
    int z;
    int dist;
    char* warppadLNG;

    int angleCamToWarppad;
    int angleSin;
    int angleCos;
    MATRIX *warppadMatrix;

    int wispMaxHeight;
    int wispRiseRate;
    int rng1;
    int rng2;

    int rewardScale;
    int rewardScale2;

    int champID;
    int champSlot;

    char randKartSpawn[8];

    int hubID;
    unsigned short modelcolorID;

    //for human reading purposes
    unsigned char ADV_CUP = 100;

    boolOpen = 0;
    gGT = sdata->gGT;
    warppadObj = t->object;
    warppadInst = t->inst;
    visInstSrc = gGT->cameraDC[0].visInstSrc;

#ifndef REBUILD_PS1
    while (visInstSrc[0] != 0)
    {
        if (visInstSrc[0] == warppadInst)
        {
            boolOpen = 1;
            break;
        }

        visInstSrc++;
    }
#else
    boolOpen = 1;
#endif

    // array of instances in warppad object
    instArr = &warppadObj->inst[0];
    warppadMatrix = &warppadInst->matrix;

    if (boolOpen == 1)
    {
        // make instances visible
        for (i = 0; i < WPIS_NUM_INSTANCES; i++)
        {
            if (instArr[i] != 0) instArr[i]->flags &= ~(0x80);
        }
    }
    else
    {
        // make instances invisible
        for (i = 0; i < WPIS_NUM_INSTANCES; i++)
        {
            if (instArr[i] != 0) instArr[i]->flags |= 0x80;
        }
    }

    // This is the red triangle in DCxDemo's
    // level viewer, make it invisible.
    // Instance only exists for debugging
    warppadInst->flags |= 0x80;

    driver = gGT->drivers[0];
    driverInst = driver->instSelf;

    x = warppadMatrix->t[0] - driverInst->matrix.t[0];
    y = warppadMatrix->t[1] - driverInst->matrix.t[1];
    z = warppadMatrix->t[2] - driverInst->matrix.t[2];
    dist = x*x + y*y + z*z;

    levelID = warppadObj->levelID;
    hubID = gGT->levelID - GEM_STONE_VALLEY;

    // if near a portal
    // usually checks specific level ids, but why not just check current hub?
    // all warp pads that need special casing are in hub 0, gem stone valley
    if (   (   hubID != 0
            && dist < 0x144000)
        || (   hubID == 0
            && dist < 0x90000)
    )
    {
        // if you are near a new warppad, or if you already were
        // determined as near the same warppad in the last frame,
        // then use this warppad as the "closest". Otherwise the
        // game could run this for two warppads right next to each other
        if (   D232.levelID == -1
            || D232.levelID == levelID
        )
        {
            // saved as nearest warppad
            D232.levelID = levelID;

            // if not giving Aku Hint
            if (sdata->AkuAkuHintState == 0)
            {
                if (levelID < ADV_CUP) // default
                {
                    warppadLNG = sdata->lngStrings[data.metaDataLEV[levelID].name_LNG];
                }
                else // gem cups
                {
                    warppadLNG = sdata->lngStrings[data.AdvCups[levelID - ADV_CUP].lngIndex_CupName];
                }

                // midpoing X,
                // 30 pixels above botttom Y
                DecalFont_DrawLine(
                    warppadLNG,
                    gGT->pushBuffer[0].rect.x + gGT->pushBuffer[0].rect.w / 2,
                    gGT->pushBuffer[0].rect.x + gGT->pushBuffer[0].rect.h - 30,
                    FONT_BIG,
                    (JUSTIFY_CENTER | ORANGE)
                );
            }

            // if track is unlocked, ignore all other ELSE-IFs
            if (instArr[WPIS_CLOSED_1S] == 0) {}
            else if (   levelID >= ADV_CUP // gem cup
                     && (sdata->advProgress.rewards[4] & 0x20000) == 0 // Dont have hint "you must have 4 tokens for a gem"
            )
            {
                // give hint "you must have 4 tokens for a gem"
                MainFrame_RequestMaskHint(0x1b, 0);
            }
            else if (   levelID < SLIDE_COLISEUM // Trophy track
                     && (sdata->advProgress.rewards[3] & 0x1000000) == 0 // Dont have hint "you must have more trophies"
                     && GET_REQUIREMENT_TYPE((instArr[WPIS_CLOSED_ITEM]->model->id)) != STATIC_KEY // required item is not KEY
            )
            {
                // give hint for "need more trophies"
                MainFrame_RequestMaskHint(2, 0);
            }
            else if (   levelID == SLIDE_COLISEUM // Slide Col
                     && (sdata->advProgress.rewards[4] & 0x40000) == 0 // Dont have hint "you must have 10 relics"
            )
            {
                // give hint for "need more trophies"
                MainFrame_RequestMaskHint(0x1C, 0);
            }
        }
    }
    else // not near portal
    {
        D232.levelID = -1;
    }

    // if warppad is locked
    if (instArr[WPIS_CLOSED_1S] != 0)
    {
        angleCamToWarppad = ratan2(
            warppadMatrix->t[0] - gGT->pushBuffer[0].pos[0],
            warppadMatrix->t[2] - gGT->pushBuffer[0].pos[2]
        );

        angleCamToWarppad = -angleCamToWarppad;

        angleSin = MATH_Sin(angleCamToWarppad);
        angleCos = MATH_Cos(angleCamToWarppad);

        if (instArr[WPIS_CLOSED_10S] == 0) // no 10s digit
        {
            instArr[WPIS_CLOSED_1S]->matrix.t[0] = warppadMatrix->t[0] + (angleCos * -0x80 >> 0xC);
            instArr[WPIS_CLOSED_1S]->matrix.t[2] = warppadMatrix->t[2] + (angleSin * -0x80 >> 0xC);

            instArr[WPIS_CLOSED_ITEM]->matrix.t[0] = warppadMatrix->t[0] + ((angleCos << 7) >> 0xC);
            instArr[WPIS_CLOSED_ITEM]->matrix.t[2] = warppadMatrix->t[2] + ((angleSin << 7) >> 0xC);
        }
        else // 10s digit
        {
            instArr[WPIS_CLOSED_ITEM]->matrix.t[0] = warppadMatrix->t[0] + (angleCos * 0xC0 >> 0xC);
            instArr[WPIS_CLOSED_ITEM]->matrix.t[2] = warppadMatrix->t[2] + (angleSin * 0xC0 >> 0xC);

            instArr[WPIS_CLOSED_X]->matrix.t[0] = warppadMatrix->t[0] + ((angleCos << 6) >> 0xC);
            instArr[WPIS_CLOSED_X]->matrix.t[2] = warppadMatrix->t[2] + ((angleSin << 6) >> 0xC);

            instArr[WPIS_CLOSED_10S]->matrix.t[0] = warppadMatrix->t[0] + (angleCos * -0x40 >> 0xC);
            instArr[WPIS_CLOSED_10S]->matrix.t[2] = warppadMatrix->t[2] + (angleSin * -0x40 >> 0xC);

            instArr[WPIS_CLOSED_1S]->matrix.t[0] = warppadMatrix->t[0] + (angleCos * -0xa0 >> 0xC);
            instArr[WPIS_CLOSED_1S]->matrix.t[2] = warppadMatrix->t[2] + (angleSin * -0xa0 >> 0xC);
        }

        warppadObj->spinRot_Prize[0] = 0;
        warppadObj->spinRot_Prize[2] = 0;

        warppadObj->spinRot_Prize[1] += FPS_HALF(0x40);

        // reuse variable,
        // end of function anyway
        warppadInst = instArr[WPIS_CLOSED_ITEM];
        #define InstArr0 warppadInst

        // converted to TEST in rebuildPS1
        ConvertRotToMatrix(
            &InstArr0->matrix,
            &warppadObj->spinRot_Prize[0]
        );

        modelcolorID = ((InstArr0->flags) >> 20) & 0xFF;
        modelID = InstArr0->model->id;

#ifndef REBUILD_PS1
        // Trophy has no specular light
        if (modelID == STATIC_TROPHY) return;

        // OG code had pointers to warppadObj->specLightXXX
        // but that was replaced with pointers to globals,
        // because the arrays didnt actually change per warppad

        // Relic
        if (modelID == STATIC_RELIC)
        {
            if (modelcolorID == RELIC_ANY)
            {
                i = (gGT->timer / FPS_DOUBLE(0x3C)) % 3;

                InstArr0->colorRGBA = (i == RELIC_SAPPHIRE)
                    ? 0x20a5ff0
                    : (i == RELIC_GOLD)
                        ? 0xd8d2090
                        : 0xffede90;
            }
            Vector_SpecLightSpin3D(
                InstArr0,
                &warppadObj->spinRot_Prize[0],
                &D232.specLightRelic[0]
            );
            return;
        }

        // Token
        if (modelID == STATIC_TOKEN)
        {
            if (modelcolorID == TOKEN_ANY)
            {
                i = (gGT->timer / FPS_DOUBLE(0x3C)) % 5;

                InstArr0->colorRGBA = (
                    ((unsigned int)data.AdvCups[i].color[0] << 0x14)
                    | ((unsigned int)data.AdvCups[i].color[1] << 0xc)
                    | ((unsigned int)data.AdvCups[i].color[2] << 0x4)
                );
            }

            Vector_SpecLightSpin3D(
                InstArr0,
                &warppadObj->spinRot_Prize[0],
                &D232.specLightToken[0]
            );
            return;
        }

        // If Gem, change colors every 2 seconds
        if (modelID == STATIC_GEM)
        {
            if (modelcolorID == GEM_ANY)
            {
                modelcolorID = (gGT->timer / FPS_DOUBLE(0x3C)) % 5;
                // gem color
                InstArr0->colorRGBA = (
                    ((unsigned int)data.AdvCups[modelcolorID].color[0] << 0x14)
                    | ((unsigned int)data.AdvCups[modelcolorID].color[1] << 0xc)
                    | ((unsigned int)data.AdvCups[modelcolorID].color[2] << 0x4)
                );
            }
        }

        // for Key or Gem
        Vector_SpecLightSpin3D(
            InstArr0,
            &warppadObj->spinRot_Prize[0],
            &D232.specLightGem[0]
        );
#endif
        return;
    }

    // === Assume Unlocked ===

    if (   instArr[WPIS_OPEN_BEAM] != 0
        && (gGT->timer & FPS_DOUBLE(1)) != 0
    )
    {
        warppadObj->spinRot_Beam[0] = 0;
        warppadObj->spinRot_Beam[2] = 0;

        // what on earth was this RNG?
        // how'd they come up with something so random, that looks so good?
        i = MixRNG_Scramble();
        warppadObj->spinRot_Beam[1] += ((short)(i >> 3) + (short)((i >> 3) / 6) * -6 + 1) * 0x200;

        // converted to TEST in rebuildPS1
        ConvertRotToMatrix(
            &instArr[WPIS_OPEN_BEAM]->matrix,
            &warppadObj->spinRot_Beam[0]
        );
    }

    wispRiseRate = FPS_HALF(0x20);

    wispMaxHeight = 0x600;

    // if close to this warppad
    if (D232.levelID != -1) wispMaxHeight = 0x400;

    for (i = 0; i < 2; i++)
    {
        if (instArr[WPIS_OPEN_RING1 + i] != 0)
        {
            warppadObj->spinRot_Wisp[i][0] = 0;
            warppadObj->spinRot_Wisp[i][2] = 0;

            warppadObj->spinRot_Wisp[i][1] += FPS_HALF(0x100);

            // converted to TEST in rebuildPS1
            ConvertRotToMatrix(
                &instArr[WPIS_OPEN_RING1+i]->matrix,
                &warppadObj->spinRot_Wisp[i][0]
            );

            // if height hasn't reached max height
            if (instArr[WPIS_OPEN_RING1 + i]->matrix.t[1] < (warppadInst->matrix.t[1] + wispMaxHeight))
            {
                instArr[WPIS_OPEN_RING1 + i]->matrix.t[1] += wispRiseRate;

                // if height hasn't reached 4x RiseRate,
                // first 4 frames of rising
                if (instArr[WPIS_OPEN_RING1 + i]->matrix.t[1] < (warppadInst->matrix.t[1] + wispRiseRate * 4))
                {
                    // reduce transparency
                    instArr[WPIS_OPEN_RING1 + i]->alphaScale -= 0x380;
                }
                else // after first 4 frames
                {
                    // add transparency as the wisp spirals upward (~0x60  per frame)
                    instArr[WPIS_OPEN_RING1 + i]->alphaScale += 0xc00 / (wispMaxHeight / wispRiseRate);
                }
            }
            else // reached max height
            {
                // reset height
                instArr[WPIS_OPEN_RING1 + i]->matrix.t[1] = warppadInst->matrix.t[1];

                // full transparency
                instArr[WPIS_OPEN_RING1 + i]->alphaScale = 0x1000;

                rng1 = MixRNG_Scramble() >> 3;

                rng2 = rng1;
                if(rng1 < 0) rng2 = rng1 + 0xfff;

                warppadObj->spinRot_Wisp[i][1] = (short)rng1 + (short)(rng2 >> 0xc) * -0x1000;
            }
        }

        wispRiseRate += FPS_HALF(0x10);
    }

    warppadObj->spinRot_Prize[1] += FPS_HALF(0x80);

    rewardScale = 0x100;

    if (dist > 0x900000 * 2)
    {
        rewardScale = 0;
    }
    else if (dist > 0x900000)
    {
        // range [90, 90*2] to [0%, 100%]
        rewardScale = ((((0x900000 * 2) - dist) * 0x100) / 0x900000);
    }

    struct Instance *itemInst;
    for (i = 0; i < 5; i++)
    {
        warppadObj->spinRot_Prize[2] = 0x155;

        /* Special handling for the new gold relic reward on
           instance slot WPIS_CLOSED_ITEM and the new platinum relic
           reward on instance slot WPIS_CLOSED_X
        */
        itemInst = (i < 3)
            ? instArr[WPIS_OPEN_PRIZE1 + i]
            : (i == 3)
                ? instArr[WPIS_CLOSED_ITEM]
                : instArr[WPIS_CLOSED_X]
        ;

        if (itemInst != 0)
        {
            AH_WarpPad_SpinRewards(
                itemInst,
                warppadObj,
                (i < 3) ? i : 2, // apply spin of sapphire relics to custom relic models as well
                warppadInst->matrix.t[0],
                warppadInst->matrix.t[1],
                warppadInst->matrix.t[2]
            );

            modelID = itemInst->model->id;

            if (   rewardScale == 0
                || (i >= 2 && ((gGT->timer / FPS_DOUBLE(30)) % 3) != i-2) // cycle relic rewards
            )
            {
                // invisible
                itemInst->flags |= 0x80;
            }
            else
            {
                // visible
                itemInst->flags &= ~(0x80);

                // token & gem
                rewardScale2 = 0x2000;

                // not token or gem
                switch (modelID)
                {
                    case STATIC_RELIC:
                    case STATIC_KEY:
                        rewardScale2 = 0x1800;
                        break;
                    case STATIC_TROPHY:
                        rewardScale2 = 0x2800;
                        break;
                }

                rewardScale2 = (unsigned int)(rewardScale2 * rewardScale) >> 8;
                itemInst->scale[0] = (short)rewardScale2;
                itemInst->scale[1] = (short)rewardScale2;
                itemInst->scale[2] = (short)rewardScale2;
            }
        }

        // Do not touch thirds or spinRot if current index is of custom gold or
        // platinum relic models
        if (i < 3)
        {
            warppadObj->thirds[i] += FPS_HALF(0x20);
            warppadObj->spinRot_Rewards[1] += FPS_HALF(0x4);
        }
    }

    // if flag is on-screen, loading has already been finalized
    if (RaceFlag_IsTransitioning() != 0) return;

    // if driver has not entered this warppad
    if (warppadObj->boolEnteredWarppad == 0)
    {
        if (dist > 0x8fff) // if far away from warppad, quit
        {
            return;
        }
        else // close to warppad, first frame
        {
            // now in warppad
            warppadObj->boolEnteredWarppad = 1;
            warppadObj->framesWarping = 0;
        }
    }

    // Spam every frame (not a bug, more like last-minute hack).
    // This wont have a negative performance impact cause it's IF-guarded,
    // needed cause mask hint sets state to 0xB (Freeze), then this sets
    // warp back. Without this, Freeze causes mask-grab, which makes you drive.
    // However, with this, state goes 0xA, then 0xB, then 0xA, and warp SFX
    // plays a second time. Animation also plays twice but is invisible second time
#ifndef REBUILD_PS1
    gGT->drivers[0]->funcPtrs[0] = VehStuckProc_Warp_Init;
#endif

    if (warppadObj->framesWarping < FPS_DOUBLE(0x400)) warppadObj->framesWarping++;

    // optimization, dont do this "every" frame,
    // which the original game did. Also this needs
    // to happen before the frames<61, to preserve
    // OG bug (pause->quit->adventure->load->spawn8th)
    if (warppadObj->framesWarping == 1)
    {
        // Assign Characters
        // Dont worry about Token or Relic, those dont
        // use kartSpawnOrderArray, the OG game just did
        // this without an IF check at all
        if (   levelID < SLIDE_COLISEUM
            || levelID >= ADV_CUP
        )
        {
            // assign characterIDs, not actually "load"
            LOAD_Robots1P(data.characterIDs[0]);

            // spawn P1 in the back
            sdata->kartSpawnOrderArray[0] = 7;

            // variable reuse, get track speed champion
            champID = data.metaDataLEV[levelID].characterID_Champion;

            // default
            champSlot = 0;

            // If Speed Champion is on the track (Crash-Pura)
            // and is not the same characterID as Player 1
            if (   champID < 8
                && champID != data.characterIDs[0]
            )
            {
                // set everyone to spawn in order
                for (i = 1; i < 7; i++)
                {
                    if (champID == data.characterIDs[i])
                    {
                        sdata->kartSpawnOrderArray[i] = 0;
                        champSlot = i;
                    }
                    else
                    {
                        sdata->kartSpawnOrderArray[i] = i;
                    }
                }

                sdata->kartSpawnOrderArray[7] = champSlot;
            }
            else // Speed Champion is invalid
            {
                for (i = 1; i < 8; i++) randKartSpawn[i] = i;

                for (i = 0; i < 7; i++)
                {
                    #ifndef REBUILD_PS1
                    rng1 = RngDeadCoed(&sdata->const_0x30215400);
                    #else
                    rng1 = 0;
                    #endif

                    rng2 = 7 - i;

                    rng2 = (rng1 & 0xfff) % rng2 + 1;
                    rng2 = (short) rng2;

                    sdata->kartSpawnOrderArray[randKartSpawn[rng2]] = (char) i;

                    while (rng2 < 7)
                    {
                        randKartSpawn[rng2] = randKartSpawn[rng2 + 1];
                        rng2++;
                    }
                }
            }
        }
    }

    // wait 2 full seconds before loading
    if (warppadObj->framesWarping <= FPS_DOUBLE(60)) return;

    // only works for trophy tracks rn
    if (levelID < SLIDE_COLISEUM)
    {
        // if trophy is unlocked
        if (CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 6)) != 0)
        {
            // if never opened
            if (sdata->boolOpenTokenRelicMenu == 0)
            {
                D232.menuTokenRelic.rowSelected = (
                    CHECK_ADV_BIT(sdata->advProgress.rewards, (levelID + 0x4c)) != 0
                );

                // now opened
                sdata->boolOpenTokenRelicMenu = 1;

                RECTMENU_Show(&D232.menuTokenRelic);

                // dont load level
                return;
            }

            // if opened, but not closed yet
            if ((RECTMENU_BoolHidden(&D232.menuTokenRelic) & 0xffff) == 0)
            {
                // dont load level
                return;
            }
            else // if opened, then closed
            {
                // Relic Hint
                i = 0x1d;

                // CTR Token Hint
                if ((gGT->gameMode2 & 8) != 0) i = 0x1a;

                // if hint is locked
                if (CHECK_ADV_BIT(sdata->advProgress.rewards, (i + 0x76)) == 0)
                {
                    MainFrame_RequestMaskHint(i, 1);
                }

                // if can't spawn aku cause he's already here,
                // quit function, wait till he's done to start race
                i = AH_MaskHint_boolCanSpawn();
                if ((i & 0xffff) == 0) return;

                // reset for future gameplay
                sdata->boolOpenTokenRelicMenu = 0;
            }
        }
    }
    else if (levelID < NITRO_COURT) // Slide Col or Turbo Track
    {
        // Add Relic
        sdata->Loading.OnBegin.AddBitsConfig0 |= RELIC_RACE;

        // RANDOMIZER
        // Apparently the comment block further up is wrong for our case, and
        // relic races *do* care about the kartSpawnOrderArray.
        // So always place player in pos 1 if relic race.
        sdata->kartSpawnOrderArray[0] = 1;
    }
    else if (levelID < GEM_STONE_VALLEY) // Battle Tracks
    {
        // Add Crystal Challenge
        sdata->Loading.OnBegin.AddBitsConfig0 |= CRYSTAL_CHALLENGE;

        // Dont have hint "collect every crystal"
        if ((sdata->advProgress.rewards[4] & 0x8000) == 0) MainFrame_RequestMaskHint(0x19, 1);

        // if can't spawn aku cause he's already here,
        // quit function, wait till he's done to start race
        i = AH_MaskHint_boolCanSpawn();
        if ((i & 0xffff) == 0) return;

        gGT->originalEventTime = D232.timeCrystalChallenge[levelID - NITRO_COURT];
    }
    else // gem cups
    {
        // Add Adv Cup
        sdata->Loading.OnBegin.AddBitsConfig0 |= ADVENTURE_CUP;

        gGT->cup.cupID = levelID - ADV_CUP;
        gGT->cup.trackIndex = 0;
        for(i = 0; i < 8; i++) gGT->cup.points[i] = 0;

        levelID = data.advCupTrackIDs[4*gGT->cup.cupID];
    }

    if ((gGT->gameMode1 & RELIC_RACE) != 0)
    {
        // RANDOMIZER
        // Apparently the comment block further up is wrong for our case, and
        // relic races *do* care about the kartSpawnOrderArray.
        // So always place player in pos 1 if relic race.
        sdata->kartSpawnOrderArray[0] = 1;
    }

    // Save current level ID for returning here after a gem cup
    pre_gemcup_levelid = gGT->levelID;

    // Rem Adventure Arena
    sdata->Loading.OnBegin.RemBitsConfig0 |= ADVENTURE_ARENA;

    MainRaceTrack_RequestLoad(levelID);
}
