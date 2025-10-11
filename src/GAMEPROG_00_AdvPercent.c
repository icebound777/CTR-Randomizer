#include <common.h>

#include "saveslot_defines.h"

void GAMEPROG_AdvPercent(struct AdvProgress* adv)
{
    int i;
    int percent;
    int numGems;
    int bitIndex;
    struct MetaDataLEV* mdLev;
    struct GameTracker* gGT;
    struct AdvProgress *advSlot2 = ((struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4));
    struct AdvProgress *advSlot3 = ((struct AdvProgress*) (sdata->memcardBytes + 0xA0 + 4));
    gGT = sdata->gGT;
    mdLev = &data.metaDataLEV[0];

    // start counter
    percent = 0;
    numGems = 0;

#if 0
    // erase counters
    for(i = 0; i < 9; i++)
    {
        ((int*)&gGT->currAdvProfile.numTrophies)[i] = 0;
    }

    // check all tracks generically
    for(i = 0; i < 18; i++)
    {
        // first bit of blue relic
        bitIndex = 0x16 + i;
        if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            gGT->currAdvProfile.numRelics++;
        }

        // check 16 trophies
        if(i < 0x10)
        {
            // first bit of trophy
            bitIndex = 6 + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                gGT->currAdvProfile.numTrophies++;
            }

            // first bit of token
            bitIndex = 0x4c + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                // increment number of tokens, based on
                // the tokenID of this level (red, green, blue, etc)
                ((int*)&gGT->currAdvProfile.numCtrTokens.red)[
                    mdLev[i].ctrTokenGroupID
                ]++;

                // increment number of total tokens
                gGT->currAdvProfile.numCtrTokens.total++;
            }
        }

        // check 4 keys, and 4 purple tokens
        if(i < 4)
        {
            // first bit of key
            bitIndex = 0x5e + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                gGT->currAdvProfile.numKeys++;
            }

            // first bit of purple tokens
            bitIndex = 0x6f + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                gGT->currAdvProfile.numCtrTokens.purple++;
            }
        }

        // check 5 gems
        if(i < 5)
        {
            // first bit of gem
            bitIndex = 0x6a + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                numGems++;
            }
        }

        // if beat oxide once, add 2% for first timie (2-0=2%)
        // if beat oxide twice, add 1% for second time (2-1=1%)
        if(i < 2)
        {
            // first bit of beating oxide
            bitIndex = 0x73 + i;
            if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
            {
                percent += 2-i;
            }
        }
    }
#endif

    // if beat oxide once, add 2% for first timie (2-0=2%)
    // if beat oxide twice, add 1% for second time (2-1=1%)
    for (i = 0; i < 2; i++)
    {
        // first bit of beating oxide
        bitIndex = 0x73 + i;
        if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            percent += (2 - i);
        }
    }

    // assume all tracks have
    // gold or platinum relic
    percent += 1;

    // check all tracks just for relics
    for(i = 0; i < 18; i++)
    {
        // first bit of gold relic
        bitIndex = 0x28 + i;
        if(CHECK_ADV_BIT(adv->rewards, bitIndex) != 0)
        {
            // check next relic
            continue;
        }

        // if relic is not unlocked,
        // then extra 1% is not earned
        percent -= 1;

        // stop checking relics
        break;
    }

    percent += (
        (advSlot2->SLOT2_NUM_RELICS + advSlot3->SLOT2_NUM_RELICS) * 2
        + (advSlot2->SLOT2_NUM_TROPHIES + advSlot3->SLOT2_NUM_TROPHIES) * 2
        + advSlot2->SLOT2_NUM_KEYS + advSlot3->SLOT2_NUM_KEYS
        + (advSlot2->SLOT2_NUM_TOKENS_RED) + (advSlot3->SLOT2_NUM_TOKENS_RED)
        + (advSlot2->SLOT2_NUM_TOKENS_GREEN) + (advSlot3->SLOT2_NUM_TOKENS_GREEN)
        + (advSlot2->SLOT2_NUM_TOKENS_BLUE) + (advSlot3->SLOT2_NUM_TOKENS_BLUE)
        + (advSlot2->SLOT2_NUM_TOKENS_YELLOW) + (advSlot3->SLOT2_NUM_TOKENS_YELLOW)
        + (advSlot2->SLOT2_NUM_TOKENS_PURPLE) + (advSlot3->SLOT2_NUM_TOKENS_PURPLE)
        + (advSlot2->SLOT2_NUM_GEMS) + (advSlot3->SLOT2_NUM_GEMS)
    );
#if 0
    if (   ( ( (struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4))->rewards[3] == (unsigned int) 0x3FC00000)
        && ( ( (struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4))->rewards[4] == (unsigned int) 0xFF1F0)
    )
    {
        DecalFont_DrawLine("EQUAL", 10, 150, FONT_SMALL, ORANGE);
    }
    else
    {
        DecalFont_DrawLine("NOT EQUAL NOT EQUAL NOT EQUAL", 10, 150, FONT_SMALL, ORANGE);
        //( (struct AdvProgress*) (sdata->memcardBytes[0x50 + 4]))->rewards[3] = 0x3FC00000;
        //( (struct AdvProgress*) (sdata->memcardBytes[0x50 + 4]))->rewards[4] = 0xFF1F0;
    }
    char tmp_buff[40] = {0};
    //sprintf(tmp_buff, "adv %x", (unsigned int) adv);
    //DecalFont_DrawLine(tmp_buff, 10, 160, FONT_SMALL, ORANGE);
    //sprintf(tmp_buff, "ptrToMemcardBuffer2 %x", (unsigned int) sdata->ptrToMemcardBuffer2);
    //DecalFont_DrawLine(tmp_buff, 10, 170, FONT_SMALL, ORANGE);
    sprintf(tmp_buff, "memcardBytes rew 3 %x", (unsigned int) ( (struct AdvProgress*) (sdata->memcardBytes + 0x0 + 4))->rewards[3]);
    DecalFont_DrawLine(tmp_buff, 10, 170, FONT_SMALL, ORANGE);
    sprintf(tmp_buff, "adv rew 3 %x", (unsigned int) adv->rewards[3]);
    DecalFont_DrawLine(tmp_buff, 10, 180, FONT_SMALL, ORANGE);
    //if ((int) sdata->memcard_ptrStart == 0x8008d474)
    //{
    //    DecalFont_DrawLine("YAY IS MEMCARD", 10, 100, FONT_SMALL, ORANGE);
    //}
    //else
    //{
    //    DecalFont_DrawLine("NOT MEMCARD", 10, 100, FONT_SMALL, ORANGE);
    //    char tmp_buff[20] = {0};
    //    sprintf(tmp_buff, "%x", (unsigned int) sdata->memcard_ptrStart);
    //    DecalFont_DrawLine(tmp_buff, 10, 120, FONT_SMALL, ORANGE);
    //}


    /*
    while in the main menu, adv is 800992E8
    while in adv mode, adv is 8008FBA4
    sdata->ptrToMemcardBuffer1 is 800992F0
    according to regionsEXE, memcardBytes should be at 800992E4, but for us is at 800992E4
    sdata->memcardBytes == MemcardProfile,
        so  sdata->memcardBytes + 4 should be advProgress[0],
        and sdata->memcardBytes + 0x50 + 4 should be advProgress[1],

    ( (struct AdvProgress*) (sdata->memcardBytes + 0x50 + 4)) == AdvProgress[slot2]
    */
#endif

    gGT->currAdvProfile.completionPercent = percent;
}

/*
    pointer to memcard data:
    PTR_DAT_8008d474


    // loop counter
    uVar21 = 0;

    // determine even or odd
    uVar24 = 0;

    // for uVar21 = 0; uVar21 < 4; uVar21++
    // Draw all Adventure Profiels
    do {
        iVar10 = (int)(uVar21 << 0x10) >> 0x10;

        // 8008d474 is ptr to memcard data

        // Draw Adventure Profile
        SelectProfile_DrawAdvProfile(
            struct AdvProgress *adv => PTR_DAT_8008d474 + iVar10 * 0x50 + 4,
            int posX                => (int)((uVar24 * 0xea + 0x1a) * 0x10000) >> 0x10,
            int posY                => ((iVar10 - ((int)(uVar21 << 0x10) >> 0x1f) >> 1) * 0x43 + 0x3c) * 0x10000 >> 0x10,
            u_int isHighlighted     => (uint)(iVar10 == (int)*(short *)(param_1 + 0x1a)),
            short slotIndex         => iVar10,
            u_short menuFlag        => (int)*(short *)(param_1 + 0x14)
        );

        // increment loop counter
        uVar21 = uVar21 + 1;

        // determine even or odd (left side of screen or right)
        uVar24 = uVar21 & 1;
    } while ((int)(uVar21 * 0x10000) >> 0x10 < 4);

    note:
    advProgress locations for 4 slots:
    1 -> PTR_DAT_8008d474        + 4
    2 -> PTR_DAT_8008d474 + 0x50 + 4
    2 -> PTR_DAT_8008d474 + 0xA0 + 4
    2 -> PTR_DAT_8008d474 + 0xF0 + 4
*/