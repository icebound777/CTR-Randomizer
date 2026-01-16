#include <common.h>

void DECOMP_RR_EndEvent_DrawMenu(void)
{
    struct GameTracker *gGT;
    struct AdvProgress *adv;
    struct Driver *d;
    struct Instance *relic;

    char boolEarly;
    short pos[2];
    u_int elapsedFrames;
    u_int bitIndex;
    u_int txtColor;
    RECT box;

    int iVar2;
    short sVar3;
    short startY;
    short endX;
    short sVar6;
    int startX;
    int endY;
    int uVar11;
    char auStack72[16];

    gGT = sdata->gGT;
    d = gGT->drivers[0];
    relic = sdata->ptrRelic;
    adv = &sdata->advProgress;

    // testing
    //d->numTimeCrates = gGT->timeCratesInLEV;

    // change color
    txtColor = (gGT->timer & FPS_DOUBLE(1))
        ? 0xffff8000
        : 0xffff8004;

    // 0x3a is the bit index of where platinum
    // relics start in adventure progress
    bitIndex = gGT->levelID + 0x3a;

    // set color of relic in Instance
    relic->colorRGBA =

        // check if platinum is unlocked, set platinum color
        (CHECK_ADV_BIT(adv->rewards, bitIndex)) ? 0xffede90 :

        // check if gold is unlocked, set gold color
        (CHECK_ADV_BIT(adv->rewards, (bitIndex - 0x12) )) ? 0xd8d2090 :

        // if sapphire, keep original color
        relic->colorRGBA;

    sdata->ptrTimebox1->scale[0] = 0x300;
    sdata->ptrTimebox1->scale[1] = 0x300;
    sdata->ptrTimebox1->scale[2] = 0x300;

    // If race ended less than 900 seconds ago (30 seconds)
    if (sdata->framesSinceRaceEnded < FPS_DOUBLE(900))
    {
        // increment frame counter
        sdata->framesSinceRaceEnded++;
    }

    if (sdata->framesSinceRaceEnded > FPS_DOUBLE(509))
    {
        // start drawing the high score menu that shows the top 5 best times
        gGT->gameModeEnd |= DRAW_HIGH_SCORES;
    }



    // Did not get all crates, prepare skips in the menus
    if(d->numTimeCrates != gGT->timeCratesInLEV)
    {
        // if race ended 59-80 frames ago
        if ((u_int)(sdata->framesSinceRaceEnded - FPS_DOUBLE(21)) < FPS_DOUBLE(59))
        {
            // advance timer to 140 frames, since we can skip the amount of time
            // that would have been taken to draw "PERFECT" text
            sdata->framesSinceRaceEnded = FPS_DOUBLE(140);
        }

        // if race ended 229-250 frames ago, and if WON relic
        if (
                ((gGT->gameModeEnd & NEW_RELIC) == 0) &&
                ((u_int)(sdata->framesSinceRaceEnded - FPS_DOUBLE(21)) < FPS_DOUBLE(229))
            )
        {
            // advance timer to 370 frames, since we can skip the amount of time
            // that would have been taken to draw the animation
            // to deduct 10 seconds from the relic timer
            sdata->framesSinceRaceEnded = FPS_DOUBLE(370);
        }
    }



    // Draw Race Clock,
    // Reset local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;
    {
        if (elapsedFrames >= FPS_DOUBLE(490))
        {
            elapsedFrames -= FPS_DOUBLE(490);

            startX = 0x100;
            endY = -0x32;
        }

        // 0 - 489
        else
        {
            startX = -0x96;
            endY = 0x32;
        }


        // interpolate fly-in
        DECOMP_UI_Lerp2D_Linear(
            &pos[0],
            startX, 0x32,
            0x100, endY,
            elapsedFrames, FPS_DOUBLE(0x14));

        DECOMP_UI_DrawRaceClock(pos[0], pos[1] - 8, 1, d);
    }



    // Draw Relic,
    // Reset local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;

    if ((gGT->gameModeEnd & NEW_RELIC) != 0)
    {
        // default
        pos[0] = 0x100;

        if (elapsedFrames >= FPS_DOUBLE(490))
        {
            elapsedFrames -= FPS_DOUBLE(490);

            DECOMP_UI_Lerp2D_Linear(
                &pos[0],
                0x100, 0,
                -0x64, 0,
                elapsedFrames, FPS_DOUBLE(0x14));
        }

        else if (elapsedFrames >= FPS_DOUBLE(250))
        {
            // on exactly the 251st frame after race ends
            if (elapsedFrames == FPS_DOUBLE(251))
            {
                // play sound of unlocking relic
                OtherFX_Play(0x67, 1);
            }

            // if relic has not fully grown
            if (relic->scale[0] < 0xc00)
            {
                // make relic grow on x axis, y axis, and z axis
                relic->scale[0] += FPS;
                relic->scale[1] += FPS;
                relic->scale[2] += FPS;
            }
        }

        relic->matrix.t[0] = DECOMP_UI_ConvertX_2(pos[0], 0x100);
        relic->matrix.t[1] = DECOMP_UI_ConvertY_2(0xa2, 0x100);
    }


    // Draw Time Crates
    // Reset local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;
    {
        if (elapsedFrames >= FPS_DOUBLE(490))
        {
            elapsedFrames -= FPS_DOUBLE(490);

            // interpolate fly-in
            DECOMP_UI_Lerp2D_Linear(
                &pos[0],
                200,   0x79,
                0x264, 0x79,
                elapsedFrames, FPS_DOUBLE(0x14));
        }

        else
        {
            pos[0] = 200;
        }

        pos[1] = 0x79;

        sdata->ptrTimebox1->matrix.t[0] = DECOMP_UI_ConvertX_2(pos[0], 0x100);
        sdata->ptrTimebox1->matrix.t[1] = DECOMP_UI_ConvertY_2(pos[1], 0x100);

        // Draw 'x' before number of crates
        DecalFont_DrawLine("x", pos[0] + 0x14, pos[1] - 10, 2, 0);

        // %2.02d/%ld: Amount of crates you collected / Total number of crates
        sprintf(auStack72, "%2.02d/%ld", d->numTimeCrates, gGT->timeCratesInLEV);

        // Draw amount of crates collected
        DecalFont_DrawLine(auStack72, pos[0] + 0x21, pos[1] - 0xe, 1, 0);
    }



    // if collected all time boxes in level
    if(d->numTimeCrates == gGT->timeCratesInLEV)
    {
        // copy to local frame counter
        elapsedFrames = sdata->framesSinceRaceEnded;

        // PERFECT text, fade-in and fade-out
        if (elapsedFrames >= FPS_DOUBLE(80))
        {
            elapsedFrames -= FPS_DOUBLE(80);

            // fade-out PERFECT
            // 170 frames after the first 80
            if (elapsedFrames >= FPS_DOUBLE(170))
            {
                startX = 0x100;
                endX = 0x296;
            }

            // === fade-in PERFECT >=80 ===
            else
            {
                startX = -0x96;
                endX = 0x100;

                // 0 frames after the first 80
                if (elapsedFrames == 0)
                {
                    OtherFX_Play(0x65, 1);
                }
            }

            DECOMP_UI_Lerp2D_Linear(
                &pos[0],
                startX, 0,
                endX, 0,
                elapsedFrames, FPS_DOUBLE(0x14));

            // "PERFECT"
            DecalFont_DrawLine(
                sdata->lngStrings[0x162], pos[0], 0x8a, 1, txtColor);
        }

        // copy to local frame counter
        elapsedFrames = sdata->framesSinceRaceEnded;

        // fade-in COUNTDOWN (-10, -9, -8...)
        if (elapsedFrames >= FPS_DOUBLE(140))
        {
            elapsedFrames -= FPS_DOUBLE(140);

            // -10
            char* str = (char*)0x1f800000;
            str[0] = '-';
            str[1] = '1';
            str[2] = '0';
            str[3] = 0;

            // interpolate fly-in
            DECOMP_UI_Lerp2D_Linear(
                &pos[0],
                0x296, 0,
                0x199, 0,
                elapsedFrames, FPS_DOUBLE(0x14));

            // 20 frames after fly-in starts, do the countdown
            if (elapsedFrames >= FPS_DOUBLE(20))
            {
                elapsedFrames -= FPS_DOUBLE(20);

                // 10, 9, 8, 7...
                // changes once every 5 frames
                int minusSeconds = elapsedFrames / FPS_DOUBLE(5);

                // -3, -2, -1, -0... (dont go past 0)
                if (minusSeconds > 10)
                    minusSeconds = 10;

                // "if != 0" means
                // "if text is not -10"
                else if (minusSeconds != 0)
                {
                    // on every 5th frame, except the first frame
                    if (elapsedFrames % FPS_DOUBLE(5) == 0)
                    {
                        // subtract a second
                        d->timeElapsedInRace -= 960;
                        OtherFX_Play(99, 1);
                    }

                    // change string to match new -X
                    str[1] = '0' + (10-minusSeconds);
                    str[2] = 0;
                }
            }

            // Draw String
            DecalFont_DrawLine(str, pos[0], 0x2a, 1, txtColor);
        }
    }



    // Draw RELIC AWARDED
    // copy to local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;

    if (
            (elapsedFrames >= FPS_DOUBLE(250)) &&
            ((gGT->gameModeEnd & NEW_RELIC) != 0)
        )
    {
        // Fade-out early, so "NEW HIGH SCORE" can fade-in
        if (elapsedFrames >= FPS_DOUBLE(370))
        {
            elapsedFrames -= FPS_DOUBLE(370);

            startX = 0x100;
            endX = 0x296;
        }

        // Fade-In
        else
        {
            elapsedFrames -= FPS_DOUBLE(250);

            startX = -0x96;
            endX = 0x100;
        }

        // interpolate fly-in
        DECOMP_UI_Lerp2D_Linear(
            &pos[0],
            startX, 0x50,
            endX, 0x50,
            elapsedFrames, FPS_DOUBLE(0x14));

        // "RELIC AWARDED!"
        DecalFont_DrawLine(
            sdata->lngStrings[0x160], pos[0], pos[1], 1, txtColor);
    }




    // copy to local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;

    if (
            (elapsedFrames >= FPS_DOUBLE(370)) &&
            ((gGT->gameModeEnd & NEW_HIGH_SCORE) != 0)
        )
    {
        elapsedFrames -= FPS_DOUBLE(370);

        // 120 frames after the 370 initial frames
        if (elapsedFrames >= FPS_DOUBLE(120))
        {
            elapsedFrames -= FPS_DOUBLE(120);

            startX = 0x100;
            endX = 0x296;
        }

        else
        {
            startX = -0x96;
            endX = 0x100;
        }

        // Interpolate fly-in
        DECOMP_UI_Lerp2D_Linear(
            &pos[0],
            startX, 0x50,
            endX, 0x50,
            elapsedFrames, FPS_DOUBLE(0x14));

        // "NEW HIGH SCORE!"
        DecalFont_DrawLine(
            sdata->lngStrings[0x161], pos[0], pos[1], 1, txtColor);
    }




    // copy to local frame counter
    elapsedFrames = sdata->framesSinceRaceEnded;

    pos[1] = 0xc;

    // if race ended more than 490 frames ago
    if (elapsedFrames >= FPS_DOUBLE(490))
    {
        elapsedFrames -= FPS_DOUBLE(490);

        // Interpolate, vertical fly-out
        DECOMP_UI_Lerp2D_Linear(
            &pos[0],
            -0xa, 0xc,
            -0xa, -0x58,
            elapsedFrames, FPS_DOUBLE(0x14));
    }




    // This is actually a RECT on the stack
    box.x = -0xa;
    box.y = pos[1];
    box.w = 0x214;
    box.h = 0x3b;

    // Draw 2D Menu rectangle background
    RECTMENU_DrawInnerRect(
        &box, 0, gGT->backBuffer->otMem.startPlusFour);



    if (     // If you have not pressed X to continue
            ((sdata->menuReadyToPass & 1) == 0) &&

            (sdata->framesSinceRaceEnded >= 510) &&

            ((gGT->gameModeEnd & NEW_HIGH_SCORE) == 0)
        )
    {
        DECOMP_RR_EndEvent_DrawHighScore(0x100, 10);

        // PRESS * TO CONTINUE
        DecalFont_DrawLine(sdata->lngStrings[0xc9], 0x100, 0xbe, 1, 0xffff8000);

        if ((sdata->AnyPlayerTap & (BTN_CROSS | BTN_CIRCLE)) != 0)
        {
            RECTMENU_ClearInput();
            RECTMENU_Show(&data.menuRetryExit);

            // record that you have pressed X to continue
            sdata->menuReadyToPass |= 1;
        }
    }
}