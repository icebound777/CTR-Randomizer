#include <common.h>
/* START Randomizer */
#include "saveslot_defines.h"
#include "CTRRandomizer_database.h"
/* END Randomizer */

void AH_Garage_Open(struct ScratchpadStruct *, struct Thread *);

void AH_Garage_ThTick(struct Thread *t)
{
    char bossIsOpen, i;
    char levelID;
    char hubID;
    int top;
    int move;
    int ratio;
    int bottom;
    short *check;
    #if 0 /* RANDOMIZER */
    u_int bitIndex;
    #endif
    u_int uVar5;
    u_int uVar8;
    int dist[3];
    int pos[3];
    struct BossGarageDoor *garage;
    struct Instance *inst;
    struct Instance *drv_inst;
    struct GameTracker *gGT;
    struct AdvProgress *adv;

    /* START Randomizer */
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));
    /* END Randomizer */

    bossIsOpen = true;

    gGT = sdata->gGT;
    adv = &sdata->advProgress;
    garage = t->object;
    inst = t->inst;
    drv_inst = gGT->drivers[0]->instSelf;
    levelID = gGT->levelID;
    hubID = levelID - GEM_STONE_VALLEY;

    // if door is not opening or closing
    if (garage->direction == 0)
    {
        // if door is fully closed
        if (garage->cooldown == 0)
        {
            inst->flags |= 0x1000;
            inst->flags &= 0xffcfdfff;
        }

        // if door is not fully closed
        else
        {
            // subtract frame timer
            garage->cooldown -= gGT->elapsedTimeMS;

            // if countdown is not done, dont close door
            if (garage->cooldown > 0)
                goto LAB_800aeb6c;

            // play sound of normal boss door opening, except for Oxide
            uVar8 = (levelID == GEM_STONE_VALLEY) ? 0x96 : 0x95;

            // Play sound
            OtherFX_Play(uVar8, 1);

            // erase cooldown
            garage->cooldown = 0;

            // door is closing
            garage->direction = -1;

            inst->flags &= 0xffffff7f;
        }
    }
    // if door is opening or closing
    else
    {
        // Increment animation by 0x20 in either direction
        move = inst->matrix.t[1] + garage->direction * FPS_HALF(0x20);
        inst->matrix.t[1] = move;

        top = inst->instDef->pos[1] + 0x300;
        bottom = inst->instDef->pos[1];

        // If the door has gone past the top (height=0x300)
        if (move > top)
        {
            // Set position to the top
            inst->matrix.t[1] = top;

            // Door is now open (not moving)
            garage->direction = 0;

            // Cooldown for 2 seconds
            garage->cooldown = 0x780;

            // Make invisible
            inst->flags |= 0x80;
        }
        // If the door has gone past the bottom
        else if (move < bottom)
        {
            // Set position to the bottom
            inst->matrix.t[1] = bottom;

            garage->direction = 0;

            garage->cooldown = 0;

            // Enable door collision
            sdata->doorAccessFlags &= 0xfffffffe;
        }
        // If the door is between the top and bottom positions
        else if (garage->garageTopInst != 0)
        {
            // Update rotation of garagetop
            garage->rot[0] += (short)garage->direction * FPS_HALF(0x40);

            // converted to TEST in rebuildPS1
            ConvertRotToMatrix(
                &garage->garageTopInst->matrix,
                &garage->rot[0]);
        }

        inst->flags &= 0xffffefff;
        inst->flags |= 0x302000;
    }

LAB_800aeb6c:

    // If you're in Gemstone Valley
    if (levelID == GEM_STONE_VALLEY)
    {
        #if 0 /* RANDOMIZER */
        // ripper roo boss key
        bitIndex = 0x5e;
        #endif

        // check four boss keys
        if ((advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS) < 4)
        {
            bossIsOpen = false;
        }
        #if 0 /* RANDOMIZER */
        for (i = 0; i < 4; i++)
        {
            if (CHECK_ADV_BIT(adv->rewards, bitIndex) == 0)
                goto LAB_800aebd0;
            bitIndex++;
        }
        #endif
    }
    else // If you're not in Gemstone Valley
    {
        int garage_requirement_logic = GARAGE_OPENING_VANILLA_WARPPADS; // default
        int db_fetch_result = DB_VALUE_NOTFOUND;
        int db_ret = database_fetch(
            DB_PREFIX_SETTINGS | SETTING_BOSS_GARAGE_OPENING,
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
                bossIsOpen = false;
            }
        }
        else
        {
            check = &data.advHubTrackIDs[(levelID - N_SANITY_BEACH) * 4];
            // check all four vanilla tracks on current hub
            for (i = 0; i < 4; i++)
            {
                int levelID_to_check = check[i];

                if (garage_requirement_logic == GARAGE_OPENING_CURRENTHUB_WARPPADS)
                {
                    // Adjust check to look at the warp pads in the current hub
                    // compensating for any warp pad randomization
                    int randomized_LevelID = database_fetch(
                        DB_PREFIX_LEVELIDS | levelID_to_check,
                        &db_fetch_result
                    );
                    if (db_fetch_result == DB_VALUE_OK) levelID_to_check = randomized_LevelID;
                }

                // if any of the four trophy races is not beaten
                if (CHECK_ADV_BIT(adv->rewards, levelID_to_check + 6) == 0)
                {
                    // boss is not open
                    bossIsOpen = false;
                    break;
                }
            }
        }
    }

    dist[0] = drv_inst->matrix.t[0] - inst->instDef->pos[0];
    dist[1] = drv_inst->matrix.t[1] - inst->instDef->pos[1];
    dist[2] = drv_inst->matrix.t[2] - inst->instDef->pos[2];

    // if in a state where you're seeing the boss key open an adv door,
    // or some other kind of cutscene where you can't move
    if ((gGT->gameMode2 & 4) != 0) return;

    // check distance
    if (0x143fff < dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2])
        goto LAB_800aede0;

    RECT view = gGT->pushBuffer[0].rect;

    // if aku is not giving a hint
    if (sdata->AkuAkuHintState == 0)
    {
        // draw string, lng_challenge
        DecalFont_DrawLine(

            sdata->lngStrings[data.lng_challenge[R232.bossIDs[hubID]]],

            (view.x + view.w >> 1),
            ((view.y + view.h) - 0x1e),
            1, 0xffff8000);
    }

    if (bossIsOpen)
        goto LAB_800aede8;

    uVar8 = 0;

    // if this is gemstone valley
    if (levelID == GEM_STONE_VALLEY)
    {
        // if hint is not unlocked "need 4 keys for oxide"
        if ((sdata->advProgress.rewards[3] & 0x4000000) == 0)
            // HintID: need four keys to race oxide
            uVar8 = 4;
    }
    // not gemstone valley
    else
    {
        //  if hint is not unlocked "to access this boss garage..."
        if ((sdata->advProgress.rewards[3] & 0x2000000) == 0)
            // HintID: need four trophies to enter boss
            uVar8 = 3;
    }

    if(uVar8 != 0)
        MainFrame_RequestMaskHint(uVar8, 0);

LAB_800aede0:

    if (!bossIsOpen)
        return;

LAB_800aede8:

#define SPS \
    ((struct ScratchpadStruct *)0x1f800108)

    SPS->Input1.pos[0] = inst->instDef->pos[0];
    SPS->Input1.pos[1] = inst->instDef->pos[1];
    SPS->Input1.pos[2] = inst->instDef->pos[2];
    SPS->Input1.hitRadius = 0x300;
    SPS->Input1.hitRadiusSquared = 0x90000;
    SPS->Input1.modelID = STATIC_PINGARAGE;

    SPS->Union.ThBuckColl.thread = t;
    SPS->Union.ThBuckColl.funcCallback = AH_Garage_Open;

#ifndef REBUILD_PS1
    // Open garage door when player gets within radius of door
    PROC_CollideHitboxWithBucket(gGT->threadBuckets[PLAYER].thread, SPS, 0);
#endif

    ratio = MATH_Sin((int)inst->instDef->rot[1]);

    pos[0] = (int)inst->instDef->pos[0] + (ratio * -0x280 >> 0xc);
    pos[1] = (int)inst->instDef->pos[1];

    ratio = MATH_Cos((int)inst->instDef->rot[1]);

    pos[2] = (int)inst->instDef->pos[2] + (ratio * -0x280 >> 0xc);

    // DriverPos - DoorPos
    dist[0] = drv_inst->matrix.t[0] - pos[0];
    dist[1] = drv_inst->matrix.t[1] - pos[1];
    dist[2] = drv_inst->matrix.t[2] - pos[2];

    // If small distance (inside garage)
    if (dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2] < 0x40000)
    {
        // Fade To Black
        gGT->pushBuffer_UI.fadeFromBlack_desiredResult = 0;
        gGT->pushBuffer_UI.fade_step = -0x2AA;
    }

    // If fade complete, start loading level
    if (gGT->pushBuffer_UI.fadeFromBlack_currentValue == 0)
    {
        sdata->Loading.OnBegin.RemBitsConfig0 |= ADVENTURE_ARENA;
        sdata->Loading.OnBegin.AddBitsConfig0 |= ADVENTURE_BOSS;

        if (
            (levelID == GEM_STONE_VALLEY) &&
            ((advSlot2->SLOT2_NUM_RELICS + advSlot3->SLOT2_NUM_RELICS) == 18))
        {
            // set string index (0-5) to "N Oxide's Final Challenge"
            gGT->bossID = 5;
        }
        else
        {
            gGT->bossID = R232.bossIDs[hubID];
        }

        // new levelID
        levelID = R232.bossTracks[hubID];

        // Set the boss character (P2)
        data.characterIDs[1] = data.metaDataLEV[levelID].characterID_Boss;

        RaceFlag_SetDrawOrder(1);
        MainRaceTrack_RequestLoad(levelID);
    }
    return;
}