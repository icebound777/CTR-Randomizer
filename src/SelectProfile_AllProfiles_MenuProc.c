#include <common.h>

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

// Used to draw four Adventure Profiles
// Load, Save, Delete, etc, also used to
// draw Ghost Profiles in Time Trial

enum OtherFX_Menu {
    MENU_FX_CHANGE_SELECTION = 0,
    MENU_FX_SELECT = 1,
    MENU_FX_EXIT = 2,
    MENU_FX_ERROR = 5
};

#define BTNS_CONFIRM (BTN_CROSS_one | BTN_CIRCLE)
#define BTNS_ABORT (BTN_SQUARE_one | BTN_TRIANGLE)

// SelectProfile_AllProfiles_MenuProc
void SelectProfile_AllProfiles_MenuProc(struct RectMenu* menu)
{
    bool bVar1;
    bool bVar2;
    struct GameTracker *gGT;
    short sVar5;
    //undefined4 *puVar6;
    struct AdvProgress *curAdvProgress = &(sdata->advProgress);
    int iVar8;
    int iVar9;
    int iVar10;
    //undefined4 *puVar11;
    int iVar12;
    //undefined4 *puVar13;
    unsigned short uVar14;
    //undefined4 uVar15;
    //undefined4 uVar16;
    //undefined4 uVar17;
    unsigned int uVar18;
    unsigned short uVar19;
    unsigned int uVar21;
    short sVar22;
    int iVar23;
    unsigned int uVar24;
    short local_58;
    short local_56 [3];
    unsigned short text_color;
    unsigned short num_ghostprofiles_saved;
    short canChooseEmptySlot;

    text_color = ORANGE;
    if ((menu->drawStyle & 0x10) != 0)
    {
        text_color = LIGHT_GREEN;
    }
    uVar24 = 0;

    num_ghostprofiles_saved = sdata->numGhostProfilesSaved;

    bVar2 = false;
    canChooseEmptySlot = false;

    if (sdata->mcScreenText == MC_SCREEN_WARNING_NOCARD)
    {
        sdata->data10_bbb[4] = 0;
    }

    // NO CARD, UNFORMATTED, or FORMATTING
    if (sdata->mcScreenText < 2)
    {
        sdata->data10_bbb[1] = 0;
    }

    SelectProfile_UnMuteCursors();

    if (   sdata->data10_bbb[2] != 0
        || sdata->data10_bbb[3] != 0
        || sdata->data10_bbb[1] != 0
    )
    {
        SelectProfile_MuteCursors();
    }

    if (sdata->data10_bbb[4] != 0)
    {
        // If you press D-pad or Cross, Square, Triangle, Circle
        if (sdata->buttonTapPerPlayer[0] & (
                (BTN_UP | BTN_DOWN | BTN_LEFT | BTN_RIGHT | (BTNS_CONFIRM) | (BTNS_ABORT))
            ) != 0
        )
        {
            // If you press Up
            if (sdata->buttonTapPerPlayer[0] & BTN_UP != 0)
            {
                // If there is room to move up
                if (0 < data.menuOverwriteAdv.rowSelected)
                {
                    OtherFX_Play(MENU_FX_CHANGE_SELECTION, 1);

                    // Move up
                    data.menuOverwriteAdv.rowSelected--;
                }
            }
            else if (sdata->buttonTapPerPlayer[0] & BTN_DOWN != 0)
            {
                if (data.menuOverwriteAdv.rowSelected < 1)
                {
                    OtherFX_Play(MENU_FX_CHANGE_SELECTION, 1);

                    data.menuOverwriteAdv.rowSelected++;
                }
            }
            else
            {
                if ((sdata->buttonTapPerPlayer[0] & (BTNS_CONFIRM)) != 0)
                {
                    OtherFX_Play(MENU_FX_SELECT, 1);

                    if (data.menuOverwriteAdv.rowSelected == 0) bVar2 = true;
                }
                else // If you press Square or Triangle
                {
                    OtherFX_Play(MENU_FX_EXIT, 1);
                }

                sdata->data10_bbb[4] = 0;
            }

            RECTMENU_ClearInput();

            // menuOverwriteGhost.rowSelected
            data.menuOverwriteGhost.rowSelected = data.menuOverwriteAdv.rowSelected;
        }
        goto LAB_800499e4;
    }

    // If this is "Choose a Ghost" screen in Time Trial
    if (sdata->data10_bbb[0] == 0x30)
    {
        // If you are saving
        if (sdata->memcardAction == 1)
        {
            // if enough room remains on memory card to save ghost
            uVar24 = (unsigned int)((int)sdata->memoryCard_SizeRemaining < 0x3e00) ^ 1;

            // canChooseEmptySlot
            // is there room to save another
            canChooseEmptySlot = (short)uVar24;

            uVar24 += sdata->numGhostProfilesSaved;

            num_ghostprofiles_saved = sdata->numGhostProfilesSaved;

            // if attempted to save more than 7 ghosts
            if (7 < (int)(uVar24 * 0x10000) >> 0x10)
            {
                // assume only 7 are saved
                uVar24 = 7;

                // canChooseEmptySlot
                // can not save another
                canChooseEmptySlot = false;
            }
        }
        else // if you are loading
        {
            // canChooseEmptySlot
            // can load empty ghost
            canChooseEmptySlot = true;

            // number of ghsots saved, plus 1,
            // which should be the first "empty" slot
            uVar24 = (unsigned int)num_ghostprofiles_saved + 1;
        }

        // if your row selected is negative
        if (menu->rowSelected < 0)
        {
            // reset to zero
            menu->rowSelected = 0;
        }

        // compare against the row selected
        if (uVar24 <= menu->rowSelected)
        {
            menu->rowSelected = uVar24 - 1;
        }
    }

    if (sdata->data10_bbb[1] != 0) goto LAB_800499e4;
    iVar10 = 0;
    if ((sdata->data10_bbb[0] == 0x30) && (sdata->data10_bbb[4] == 0))
    {
        iVar10 = (int)(short)uVar24;
        iVar23 = 0;
        LAB_800495a0:

        iVar10 = SelectProfile_InputLogic(menu, iVar10, iVar23);
        LAB_800495ac:
        iVar10 = iVar10 << 0x10;
    }
    else
    {
        if (sdata->data10_bbb[0] != 0x40)
        {
            uVar14 = 0;

            if (   (sdata->mcScreenText < 2) // NO CARD, UNFORMATTED, or FORMATTING
                || (   sdata->memoryCard_SizeRemaining < 0x1680
                    && sdata->unk_memcardRelated_8008d928[0] == 0 // if memcard has no room
                    && sdata->memcardAction == 1)
            )
            {
                uVar14 = 1;
            }

            // NO CARD, UNFORMATTED, or FORMATTING
            if (sdata->mcScreenText < 2) uVar14 = uVar14 | 2;

            iVar10 = 4;
            iVar23 = (int)(short)uVar14;
            goto LAB_800495a0;
        }

        if (sdata->mcScreenText == MC_SCREEN_WARNING_NOCARD)
        {
            // If you press Cross or Circle
            if ((sdata->buttonTapPerPlayer[0] & (BTN_CROSS_one | BTN_CIRCLE)) != 0)
            {
                OtherFX_Play(MENU_FX_SELECT, 1);

                if (sdata->boolSaveCupProgress == 0)
                {
                    // MainGameEnd_SoloRaceSaveHighScore
                    //FUN_8003a2b4();
                    void (*FUN_8003a2b4)() = 0x8003a2b4;
                    (*FUN_8003a2b4)(0);
                }

                RECTMENU_ClearInput();

                sdata->data10_bbb[1] = 1;
                sdata->data10_bbb[2] = 1;
                iVar10 = 0;
                goto LAB_800495b0;
            }
            goto LAB_800495ac;
        }

        if (sdata->mcScreenText == MC_SCREEN_WARNING_UNFORMATTED)
        {
            iVar10 = 0;

            // If you press Circle
            if ((sdata->buttonTapPerPlayer[0] & BTN_CIRCLE) != 0)
            {
                RECTMENU_ClearInput();

                // OtherFX_Play
                OtherFX_Play(MENU_FX_SELECT, 1);

                // (MC_START_LOAD_MAIN)
                //FUN_80047198(7);
                void (*FUN_80047198)(short param_1) = 0x80047198;
                (*FUN_80047198)(7);

                iVar10 = 0;
            }
        }
        else
        {
            if (   (
                       (// if enough room is on memcard to save
                           (0x167f < sdata->memoryCard_SizeRemaining)
                        || (sdata->unk_memcardRelated_8008d928[0] != 0)
                       )
                    || (
                           iVar10 = 0,
                           // if not MC_START_SAVE_MAIN
                           sdata->memcardAction != 1
                       )
                   )
                && ((   sdata->unk8008d95c != 0
                     || (iVar10 = 0, sdata->unk_memcardRelated_8008d928[0] != 0)
                    ))
            )
            {
                if (sdata->boolSaveCupProgress == 0)
                {
                    // MainGameEnd_SoloRaceSaveHighScore
                    //FUN_8003a2b4();
                    void (*FUN_8003a2b4)() = 0x8003a2b4;
                    (*FUN_8003a2b4)(0);
                }

                RaceConfig_SaveGameOptions();

                GAMEPROG_SaveCupProgress();

                // params are: memcardPtr, ram ptr
                // not "save", more like "sync" memcard and ram
                GAMEPROG_SyncGameAndCard(
                    (struct GameProgress *) ((char*) sdata->ptrToMemcardBuffer2 + 0x144),
                    &(sdata->gameProgress)
                );

                // pointer to memcard GameProgress
                struct GameProgress *memGameProgress = (struct GameProgress *) ((char*) sdata->ptrToMemcardBuffer2 + 0x144);
                struct GameProgress *curGameProgress = &(sdata->gameProgress);

                memcpy(memGameProgress, curGameProgress, sizeof(struct GameProgress));
                #if 0
                do {
                    puVar11 = curGameProgress;
                    puVar6 = memGameProgress;
                    uVar15 = puVar11[1];
                    uVar16 = puVar11[2];
                    uVar17 = puVar11[3];
                    *puVar6 = *puVar11;
                    puVar6[1] = uVar15;
                    puVar6[2] = uVar16;
                    puVar6[3] = uVar17;
                    memGameProgress = puVar6 + 4;
                    curGameProgress = puVar11 + 4;
                } while (puVar11 + 4 != (short *)((int)&(sdata->gameOptions.rwd[3].deadZone) + 2));
                uVar15 = puVar11[5];
                uVar16 = puVar11[6];
                puVar6[4] = sdata->gameOptions.rwd[3].range;
                puVar6[5] = uVar15;
                puVar6[6] = uVar16;
                #endif

                // MEMCARD_SetIcon to CrashHead
                //FUN_8003d4e4(0);
                void (*FUN_8003d4e4)(short param_1) = 0x8003d4e4;
                (*FUN_8003d4e4)(0);

                // (MC_START_SAVE_MAIN)
                //FUN_80047198(3);
                void (*FUN_80047198)(short param_1) = 0x80047198;
                (*FUN_80047198)(3);

                sdata->unk_memcardRelated_8008d928[0] = 1;
                sdata->data10_bbb[1] = 1;
                iVar10 = 0;
            }
        }
    }

    LAB_800495b0:
    if (iVar10 == 0) goto LAB_800499e4;
    bVar1 = true;
    if (menu->rowSelected == -1)
    {
        sdata->data10_bbb[1] = 1;
        sdata->data10_bbb[2] = 1;
        goto LAB_800499e4;
    }

    // MC_SCREEN_WARNING_NOCARD
    if (sdata->mcScreenText == MC_SCREEN_WARNING_NOCARD)
    {
        bVar1 = false;

        // MC_START_SAVE_MAIN
        if (sdata->memcardAction == 1)
        {
            sdata->data10_bbb[1] = sdata->memcardAction;
            sdata->data10_bbb[3] = sdata->memcardAction;
            goto LAB_80049624;
        }

        // if you are not loading data
        if (sdata->memcardAction != 0) goto LAB_80049624;

        // if you are handling time trial ghosts
        if (sdata->data10_bbb[0] == 0x30)
        {
            bVar1 = true;
            goto LAB_80049624;
        }

        LAB_80049634:
        if (   (   (sdata->mcScreenText != MC_SCREEN_WARNING_UNFORMATTED)
                && (sdata->memoryCard_SizeRemaining < 0x1680) // if not enough room on memcard
               )
            && ((sdata->unk_memcardRelated_8008d928[0] == 0 && (sdata->memcardAction == 1)))
        )
        {
            bVar1 = false;
        }
    }
    else
    {
        LAB_80049624:
        if (sdata->data10_bbb[0] != 0x30) goto LAB_80049634;
    }

    if (!bVar1) goto LAB_800499e4;

    if (sdata->mcScreenText == MC_SCREEN_WARNING_UNFORMATTED)
    {
        // (MC_START_LOAD_MAIN)
        //FUN_80047198(7);
        void (*FUN_80047198)(short param_1) = 0x80047198;
        (*FUN_80047198)(7);

        goto LAB_800499e4;
    }

    if ((sdata->unk8008d95c == 0) && (sdata->unk_memcardRelated_8008d928[0] == 0)) goto LAB_800499e4;

    // if you are saving data
    if (sdata->memcardAction == 1)
    {
        // if you are handling time trial data
        if (sdata->data10_bbb[0] == 0x30)
        {
            // if lap row is less than number of ghosts
            if (menu->rowSelected < (short)sdata->numGhostProfilesSaved)
            {
                data.menuOverwriteAdv.rowSelected = 1;
                sdata->data10_bbb[4] = 1;
                data.menuOverwriteGhost.rowSelected = 1;
                sdata->ghostProfile_rowSelect = menu->rowSelected;
                goto LAB_800499e4;
            }
        }
        else // if you are handling adventure data
        {
            if (   sdata->data10_bbb[5] == 0
                && sdata->mcScreenText == MC_SCREEN_ERROR_TIMEOUT
            )
            {
                sdata->data10_bbb[5] = 1;
                goto LAB_800499e4;
            }

            // 8008d474 is ptr to memcard data
            if (   -1 < ((struct AdvProgress*) ((int) sdata->ptrToMemcardBuffer2 + (menu->rowSelected * 0x50)))->HubLevYouSavedOn
                && menu->rowSelected != sdata->advProfileIndex
            )
            {
                data.menuOverwriteAdv.rowSelected = 1;
                data.menuOverwriteGhost.rowSelected = 1;
                sdata->data10_bbb[4] = 1;
                goto LAB_800499e4;
            }
        }
        bVar2 = true;
        goto LAB_800499e4;
    }

    // If 0 or 1 (load, save)
    if (sdata->memcardAction < 2)
    {
        // if you are not loading data
        if (sdata->memcardAction != 0) goto LAB_800499e4;

        // If you are handling time trial ghosts
        if (sdata->data10_bbb[0] == 0x30)
        {
            if ((int)(short)uVar24 + -1 <= (int)menu->rowSelected)
            {
                sdata->data10_bbb[1] = 1;
                sdata->data10_bbb[3] = 1;
                memset(sdata->ptrGhostTapePlaying, 0, sizeof(struct GhostHeader));
                goto LAB_800499e4;
            }

            // if track of ghost profile matches the track selected
            if (   (int)*(short *)(&(sdata->ghostProfile_memcard[0].trackID) + (int)menu->rowSelected * 0x34)
                == // Level in Track Selection
                   sdata->gGT->currLEV
            )
            {
                // set selected ghost profile index
                sdata->ghostProfile_indexLoad = menu->rowSelected;

                // (MC_START_LOAD_GHOST)
                //FUN_80047198(5);
                void (*FUN_80047198)(short param_1) = 0x80047198;
                (*FUN_80047198)(5);

                goto LAB_800499e0;
            }
        }
        else // if you are handling adventure data
        {
            if (sdata->mcScreenText == MC_SCREEN_ERROR_TIMEOUT)
            {
                sdata->data10_bbb[1] = 1;
                sdata->data10_bbb[2] = 1;
                goto LAB_800499e4;
            }

            // 8008d474 is ptr to memcard data
            // memcard advProfile[rowSelected] HubLevYouSavedOn
            if (-1 < ((struct AdvProgress*) ((int) sdata->ptrToMemcardBuffer2 + (menu->rowSelected * 0x50)))->HubLevYouSavedOn)
            {
                // GAMEPROG_SyncGameAndCard
                // params are: memcardPtr, ram ptr
                // not "save", more like "sync" memcard and ram
                GAMEPROG_SyncGameAndCard(
                    (struct GameProgress *) ((char*) sdata->ptrToMemcardBuffer2 + 0x144),
                    &(sdata->gameProgress)
                );

                // 0x8fba4 is where the adventure profile (currently loaded) begins
                //struct AdvProgress *curAdvProgress = &(sdata->advProgress);

                // 8008d474 is ptr to memcard data
                struct MemcardProfile *memcrd_profile = (
                    (struct MemcardProfile *) (
                        (int)sdata->ptrToMemcardBuffer2 + (int)menu->rowSelected * 0x50
                    )
                );
                struct AdvProgress *memAdvProgress = &(memcrd_profile->advProgress[0]);
                memcpy(curAdvProgress, memAdvProgress, sizeof(struct AdvProgress));
                #if 0
                do {
                    uVar15 = memAdvProgress[1];
                    uVar16 = memAdvProgress[2];
                    uVar17 = memAdvProgress[3];
                    *puVar13 = *memAdvProgress;
                    puVar13[1] = uVar15;
                    puVar13[2] = uVar16;
                    puVar13[3] = uVar17;
                    uVar17 = sdata->advProgress.name[12];
                    uVar16 = sdata->advProgress.name[8];
                    uVar15 = sdata->advProgress.name[4];
                    gGT = sdata->gGT;
                    memAdvProgress = memAdvProgress + 4;
                    puVar13 = puVar13 + 4;
                } while (memAdvProgress != (undefined4 *)(memcrd_profile + 0x54));
                #endif
                // DAT_8008fbce is the Icon ID of the character you want
                // for the adventure mode character

                // Set P1 Character ID to Adventure Icon ID
                data.characterIDs[0] = sdata->advProgress.characterID;

                // related to name at OSK screen
                strncpy(sdata->gGT->prevNameEntered, sdata->advProgress.name, 18);
                #if 0
                *(char *)(sdata->gGT->prevNameEntered[0]) = sdata->advProgress.name[0];
                *(char *)(gGT->prevNameEntered[4]) = sdata->advProgress.name[4];
                *(char *)(gGT->prevNameEntered[8]) = sdata->advProgress.name[8];
                *(char *)(gGT->prevNameEntered[12]) = sdata->advProgress.name[12];
                gGT->prevNameEntered[16] = sdata->advProgress.name[16];
                #endif

                sdata->unk_8008d73C_relatedToRowHighlighted = menu->rowSelected;
                sdata->data10_bbb[1] = 1;
                sdata->data10_bbb[3] = 1;
                goto LAB_800499e4;
            }
        }

        OtherFX_Play(MENU_FX_ERROR, 1);
    }
    else // if 2 or 3 (delete or exit)
    {
        if (   (   (sdata->memcardAction != 2) // if you are not erasing data
                || (sdata->data10_bbb[0] == 0x30) // if this is time trial ghost data
               )
            || (iVar10 = (int)menu->rowSelected * 0x50,
                // 8008d474 is ptr to memcard data
                *(short *)((int)sdata->ptrToMemcardBuffer2 + iVar10 + 0x2e) < 0
               )
        ) {
            // skip deletion
            goto LAB_800499e4;
        }

        // At this point, assume you pressed Delete

        GAMEPROG_NewProfile_InsideAdv(
            (int)sdata->ptrToMemcardBuffer2 + iVar10 + 4
        );

        // MEMCARD_SetIcon to CrashHead
        //FUN_8003d4e4(0);
        void (*FUN_8003d4e4)(short param_1) = 0x8003d4e4;
        (*FUN_8003d4e4)(0);

        // (MC_START_SAVE_MAIN)
        //FUN_80047198(3);
        void (*FUN_80047198)(short param_1) = 0x80047198;
        (*FUN_80047198)(6);

        sdata->unk_memcardRelated_8008d928[0] = 1;
        LAB_800499e0:
        sdata->data10_bbb[1] = 1;
    }

    LAB_800499e4:

    if (
        (
            (sdata->mcScreenText == MC_SCREEN_ERROR_TIMEOUT) &&

            (sdata->data10_bbb[2] == 0)
        ) &&

        (sdata->data10_bbb[3] == 0)
    )
    {
        sdata->data10_bbb[1] = 0;
        sdata->data10_bbb[4] = 0;
    }

    if (bVar2)
    {
        sdata->data10_bbb[5] = 0;
        if (sdata->data10_bbb[0] == 0x30)
        {
            int racetime = 0x8ca00; // 10 minutes

            // If player structure pointer is not nullptr
            if (sdata->gGT->drivers[0] != 0)
            {
                racetime = sdata->gGT->drivers[0]->timeElapsedInRace;
            }

            // RefreshCard_GhostEncodeProfile
            //FUN_80046c30(
            //    (int)menu->rowSelected, // index highlighted by cursor
            //    data.characterIDs[0], // Character ID Array
            //    sdata->gGT->levelID,
            //    racetime, // amount of time
            //    sdata->gGT->prevNameEntered
            //);
            void (*FUN_80046c30)(
                unsigned int param_1,
                unsigned short param_2,
                unsigned short param_3,
                int param_4,
                char* param5
            ) = 0x80046c30;
            (*FUN_80046c30)(
                (int)menu->rowSelected, // index highlighted by cursor
                data.characterIDs[0], // Character ID Array
                sdata->gGT->levelID,
                racetime, // amount of time
                sdata->gGT->prevNameEntered
            );

            // index highlighted by cursor
            sdata->ghostProfile_indexSave = menu->rowSelected;

            sdata->ghostProfile_rowSelect = -1;
            if (menu->rowSelected < (short)sdata->numGhostProfilesSaved)
            {
                sdata->ghostProfile_rowSelect = menu->rowSelected;
            }

            // MEMCARD_SetIcon to GhostIcon
            //FUN_8003d4e4(1);
            void (*FUN_8003d4e4)(short param_1) = 0x8003d4e4;
            (*FUN_8003d4e4)(1);

            // (MC_START_SAVE_GHOST)
            //FUN_80047198(6);
            void (*FUN_80047198)(short param_1) = 0x80047198;
            (*FUN_80047198)(6);

            sdata->data10_bbb[1] = 1;
            sdata->gGT->gameModeEnd |= PLAYER_GHOST_BEAT;
        }
        else
        {
            sdata->unk_8008d73C_relatedToRowHighlighted = menu->rowSelected;

            RaceConfig_SaveGameOptions();

            GAMEPROG_SaveCupProgress();

            // GAMEPROG_SyncGameAndCard
            // params are: memcardPtr, ram ptr
            // not "save", more like "sync" memcard and ram
            GAMEPROG_SyncGameAndCard(
                (int)sdata->ptrToMemcardBuffer2 + 0x144,
                &(sdata->gameProgress)
            );


            // 0x8fba4 is where the adventure profile (currently loaded) begins
            //struct AdvProgress *curAdvProgress = &(sdata->advProgress);

            // 8008d474 is ptr to memcard data
            struct AdvProgress *memAdvProgress = &((struct MemcardProfile *) sdata->ptrToMemcardBuffer2)->advProgress[menu->rowSelected];

            // keep doing this while the pointer does
            // not get to the Ghost Recording Buffer
            memcpy(memAdvProgress, curAdvProgress, sizeof(struct AdvProgress));
            #if 0
            do {
                memAdvProgress[0] = curAdvProgress[0];
                memAdvProgress[1] = curAdvProgress[1];
                memAdvProgress[2] = curAdvProgress[2];
                memAdvProgress[3] = curAdvProgress[3];

                curAdvProgress = curAdvProgress + 4;
                memAdvProgress = memAdvProgress + 4;
            } while (curAdvProgress != &(sdata->GhostRecording.ptrGhost)); // end of advProgress
            #endif

            // 8008d474 is ptr to memcard data
            struct GameProgress* memProgressPtr = &(((struct MemcardProfile*) sdata->ptrToMemcardBuffer2)->gameProgress);

            struct GameProgress *curGameProgress = &(sdata->gameProgress);

            memcpy(memProgressPtr, curGameProgress, sizeof(struct GameProgress));
            #if 0
            do {
                puVar11 = curGameProgress;
                puVar6 = memProgressPtr;
                uVar15 = puVar11[1];
                uVar16 = puVar11[2];
                uVar17 = puVar11[3];
                *puVar6 = *puVar11;
                puVar6[1] = uVar15;
                puVar6[2] = uVar16;
                puVar6[3] = uVar17;
                memProgressPtr = puVar6 + 4;
                curGameProgress = puVar11 + 4;
            } while (puVar11 + 4 != (short *)((int)&(sdata->gameOptions.rwd[3].deadZone) + 2));
            uVar15 = puVar11[5];
            uVar16 = puVar11[6];
            puVar6[4] = sdata->gameOptions.rwd[3].range;
            puVar6[5] = uVar15;
            puVar6[6] = uVar16;
            #endif

            // MEMCARD_SetIcon to CrashHead
            //FUN_8003d4e4(0);
            void (*FUN_8003d4e4)(short param_1) = 0x8003d4e4;
            (*FUN_8003d4e4)(0);

            // save main CTR save file (3 = main file)
            //FUN_80047198(3);
            void (*FUN_80047198)(short param_1) = 0x80047198;
            (*FUN_80047198)(3);

            sdata->unk_memcardRelated_8008d928[0] = 1;
            sdata->data10_bbb[1] = 1;
        }

        // timerSaveComplete
        sdata->data10_bbb[6] = 0x3c;
    }

    if (*(short *)(menu + 0x1e) == 1)
    {
        bVar1 = false;
        if (   (   (sdata->data10_bbb[1] == 0)
                && (sdata->unk8008d95c != 0)
               )
            && ((   sdata->unk_memcardRelated_8008d928[0] != 0
                 || (sdata->mcScreenText == MC_SCREEN_NULL)
                ))
        )
        {
            bVar1 = true;
        }

        if (   (   (sdata->memcardAction == 0) // If you are loading data
                && (sdata->data10_bbb[0] == 0x30) // if you are handling time trial ghosts
               )
            && ((   (   sdata->mcScreenText == MC_SCREEN_ERROR_NODATA
                     || sdata->mcScreenText == MC_SCREEN_WARNING_NOCARD
                    )
                 && ((uVar24 & 0xffff) != 0) // rowSelected?
                ))
        )
        {
            bVar1 = true;
        }
        uVar14 = sdata->mcScreenText;

        // If you are saving data
        if (sdata->memcardAction == 1)
        {
            if (   sdata->mcScreenText == MC_SCREEN_NULL
                || sdata->mcScreenText == MC_SCREEN_ERROR_NODATA
            )
            {
                bVar1 = false;

                if (bVar2)
                {
                    // MC_SCREEN_SAVING
                    uVar14 = 3;
                }
                else
                {
                    // If you are handling time trial ghosts
                    if (sdata->data10_bbb[0] == 0x30)
                    {
                        bVar1 = true;

                        // if not enough room to save ghost
                        if (   ((int)sdata->memoryCard_SizeRemaining < 0x3e00)
                            && (num_ghostprofiles_saved == 0)
                        )
                        {
                            bVar1 = false;

                            // MC_SCREEN_ERROR_FULL
                            uVar14 = 6;
                        }
                    }
                    else // If you are handling adventure profiles
                    {
                        bVar1 = true;

                        // if not enough room to save profile
                        if (   sdata->memoryCard_SizeRemaining < 0x1680
                            && sdata->unk_memcardRelated_8008d928[0] == 0
                        )
                        {
                            // MC_SCREEN_ERROR_FULL
                            uVar14 = 6;
                            bVar1 = false;
                        }
                    }
                }
            }

            // MC_SCREEN_ERROR_TIMEOUT
            if ((uVar14 == 7) && (sdata->data10_bbb[5] != 0))
            {
                bVar1 = true;
            }
        }

        SelectProfile_Init((int)*(short *)(menu + 0x14));

        if ((bVar1) && (sdata->data10_bbb[1] == 0))
        {
            // if this is ghost data
            if (sdata->data10_bbb[0] == 0x30)
            {
                if (sdata->data10_bbb[4] == 0)
                {
                    uVar21 = 0;
                    //iVar10 = strlen();
                    iVar10 = 1;
                    bool local_30 = (iVar10 != 0);
                    short font_type;

                    // If number of boxes (ghosts + "no ghost")
                    // is less than 7
                    if ((short)uVar24 < 7)
                    {
                        // Make the text big
                        font_type = FONT_BIG;

                        iVar23 = 0x12;
                        iVar10 = 16;

                        // If you are not saving
                        if (sdata->memcardAction != 1)
                        {
                            // DAT_8008d878 + 0x33c
                            // INSERT ANY MEMORY CARD WITH GHOST DATA IN MEMORY CARD SLOT 1

                            DecalFont_DrawMultiLine(
                                sdata->lngStrings[0x33c],
                                0x100,
                                0xbe,
                                0x1ce,
                                2,
                                (unsigned int)text_color | 0xffff8000
                            );
                        }
                    }
                    else // If there are 7 or more boxes
                    {
                        // Make the text small
                        font_type = FONT_SMALL;

                        iVar23 = 0x12;
                        if (iVar10 != 0) iVar23 = 0xc;
                        iVar10 = 8;
                    }
                    uVar18 = (unsigned int)text_color;
                    sVar22 = (short)(iVar23 + iVar10);

                    // Top line text

                    // CHOOSE A GHOST, or
                    // CHOOSE A MEMORY CARD SLOT
                    DecalFont_DrawLine(
                        *(char *) ((int)*(short *)(&(data.lngIndex_LoadSave[0]) + (int)sdata->memcardAction * 4) * 4 + sdata->lngStrings),
                        0x100,
                        (int)(short)iVar23,
                        font_type,
                        uVar18 | 0xffff8000
                    );

                    if (local_30 != 0)
                    {
                        sVar22 = (short)iVar10 + sVar22;

                        // TO LOAD or
                        // TO SAVE TO
                        DecalFont_DrawLine(
                            *(char *) ((int)*(short *)(&(data.lngIndex_LoadSave[1]) + (int)sdata->memcardAction * 4) * 4 + sdata->lngStrings),
                            0x100,
                            (iVar23 + iVar10) * 0x10000 >> 0x10,
                            font_type,
                            uVar18 | 0xffff8000
                        );
                    }

                    // pointer to first profile
                    struct GhostProfile *curGhostProfile = &(sdata->ghostProfile_memcard[0]);

                    // numGhosts + canChooseEmptySlot
                    unsigned short local_38 = sdata->numGhostProfilesSaved + canChooseEmptySlot;

                    // Loop through all menu options
                    iVar23 = (int)(short)uVar24;
                    for (iVar10 = 0; iVar10 * 0x10000 < (int)((unsigned int)local_38 << 0x10); iVar10++)
                    {
                        if (uVar21 << 0x10 == (unsigned int)num_ghostprofiles_saved << 0x10)
                        {
                            curGhostProfile = NULL;
                        }

                        if (   ((int)(uVar21 << 0x10) >> 0x10 < iVar23 + -1)
                            || ((uVar21 & 1) != 0)
                        )
                        {
                            iVar8 = (uVar21 & 1) * 0xd4 + 0x2e;
                        }
                        else
                        {
                            iVar8 = 0x98;
                        }
                        iVar12 = ((int)(uVar21 << 0x10) >> 0x10) - ((int)(uVar21 << 0x10) >> 0x1f) >> 1;
                        if (6 < iVar23) {
                            iVar9 = iVar12 * 0x2c;
                        }
                        else
                        {
                            iVar9 = iVar12 * 0x30;
                        }
                        sVar5 = (short)uVar21;
                        //uVar24 = SEXT24(*(short *)(menu + 0x14));
                        uVar24 = *(short *)(menu + 0x14);
                        if (6 < iVar23) {
                            uVar24 = uVar24 | 0x40;
                        }
                        uVar18 = 0;
                        if (   (curGhostProfile != NULL)
                            && // Level in Track Selection
                                (curGhostProfile->trackID != sdata->gGT->currLEV)
                        )
                        {
                            // If you are not saving
                            uVar18 = (unsigned int)(sdata->memcardAction != 1);
                        }

                        // SelectProfile_DrawGhostProfile
                        //FUN_80048a30(
                        //    curGhostProfile,
                        //    iVar8,
                        //    ((int)sVar22 + 6 + (iVar9 - iVar12)) * 0x10000 >> 0x10,
                        //    (unsigned int)(sVar5 == menu->rowSelected), // bool flashing highlight
                        //    (int) sVar5,
                        //    uVar24,
                        //    (unsigned int)(sdata->memcardAction == 0), // If you are loading
                        //    uVar18
                        //);
                        void (*FUN_80048a30)(
                            int param_1,
                            short param_2,
                            short param_3,
                            short param_4,
                            short param_5,
                            unsigned short param_6,
                            short param_7,
                            short param_8
                        ) = 0x80048a30;
                        (*FUN_80048a30)(
                            curGhostProfile,
                            iVar8,
                            ((int)sVar22 + 6 + (iVar9 - iVar12)) * 0x10000 >> 0x10,
                            (unsigned int)(sVar5 == menu->rowSelected), // bool flashing highlight
                            (int) sVar5,
                            uVar24,
                            (unsigned int)(sdata->memcardAction == 0), // If you are loading
                            uVar18
                        );

                        uVar21 = uVar21 + 1;
                        if ((int)sVar5 == iVar23) break;

                        // Go to next profile
                        curGhostProfile += sizeof(struct GhostProfile);
                    }
                }
                else
                {
                    // set menu width to zero
                    local_56[0] = 0;

                    RECTMENU_GetWidth(&(data.menuOverwriteGhost), local_56, 1);

                    // draw menu
                    //FUN_80045db0(
                    //    &(data.menuOverwriteGhost),
                    //    0,
                    //    0,
                    //    (int)local_56[0]
                    //);
                    void (*FUN_80045db0)(short *param_1,int param_2,short param_3,unsigned short param_4) = 0x80045db0;
                    (*FUN_80045db0)(
                        &(data.menuOverwriteGhost),
                        0,
                        0,
                        (int)local_56[0]
                    );

                    // SelectProfile_DrawGhostProfile
                    //FUN_80048a30(
                    //    &(sdata->ghostProfile_memcard[0]) + (int)sdata->ghostProfile_rowSelect * 0x34,
                    //    0x9c,
                    //    0x3c,
                    //    0,
                    //    0,
                    //    (int)*(short *)(menu + 0x14),
                    //    0,
                    //    0
                    //);
                    void (*FUN_80048a30)(
                        int param_1,
                        short param_2,
                        short param_3,
                        short param_4,
                        short param_5,
                        unsigned short param_6,
                        short param_7,
                        short param_8
                    ) = 0x80048a30;
                    (*FUN_80048a30)(
                        &(sdata->ghostProfile_memcard[0])
                        + (int)sdata->ghostProfile_rowSelect * 0x34,
                        0x9c,
                        0x3c,
                        0,
                        0,
                        (int)*(short *)(menu + 0x14),
                        0,
                        0
                    );
                }
            }
            else // if this is adventure data
            {
                if (sdata->data10_bbb[4] == 0)
                {
                    //iVar10 = strlen();
                    iVar10 = 1;
                    uVar24 = (unsigned int)text_color;

                    // Top line text
                    // CHOOSE A GAME,
                    // CHOOSE A MEMORY CARD SLOT, or
                    // CHOOSE A GAME
                    DecalFont_DrawLine(
                        *(char *) (
                            (int)*(short *)(
                                &(data.lngStringsSaveLoadDelete[0])
                                + (int)sdata->memcardAction * 4
                            ) * 4
                            + sdata->lngStrings
                        ),
                        0x100,
                        (unsigned int)(iVar10 == 0) << 3 | 0x12, // 18 or 26
                        FONT_BIG,
                        uVar24 | 0xffff8000
                    );

                    if (iVar10 != 0)
                    {
                        // bottom line text
                        // TO LOAD,
                        // TO SAVE TO, or
                        // TO DELETE
                        DecalFont_DrawLine(
                            *(char *) (
                                (int)*(short *)(
                                    &((data.lngStringsSaveLoadDelete[1]))
                                    + (int)sdata->memcardAction * 4
                                ) * 4
                                + sdata->lngStrings
                            ),
                            0x100,
                            0x22,
                            FONT_BIG,
                            uVar24 | 0xffff8000
                        );
                    }

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
                            (int)sdata->ptrToMemcardBuffer2 + iVar10 * 0x50 + 4,
                            (int)((uVar24 * 0xea + 0x1a) * 0x10000) >> 0x10,
                            ((iVar10 - ((int)(uVar21 << 0x10) >> 0x1f) >> 1) * 0x43 + 0x3c) * 0x10000 >> 0x10,
                            (unsigned int)(iVar10 == (int)menu->rowSelected),
                            iVar10,
                            (int)*(short *)(menu + 0x14)
                        );

                        // increment loop counter
                        uVar21 = uVar21 + 1;

                        // determine even or odd (left side of screen or right)
                        uVar24 = uVar21 & 1;
                    } while ((int)(uVar21 * 0x10000) >> 0x10 < 4);
                }
                else
                {
                    // set width to zero
                    local_58 = 0;

                    // Open "Overwrite" menu

                    RECTMENU_GetWidth(&(data.menuOverwriteAdv),&local_58,1);

                    // draw menu
                    //FUN_80045db0(&(data.menuOverwriteAdv),0,0,(int)local_58);
                    void (*FUN_80045db0)(short *param_1,int param_2,short param_3,unsigned short param_4) = 0x80045db0;
                    (*FUN_80045db0)(&(data.menuOverwriteAdv),0,0,(int)local_58);

                    // 8008d474 is ptr to memcard data
                    // Draw Adventure Profile
                    SelectProfile_DrawAdvProfile(
                        (int)sdata->ptrToMemcardBuffer2 + ((int)menu->rowSelected * 0x50) + 4,
                        0x92,
                        0x3c,
                        0,
                        (int)menu->rowSelected,
                        (int)*(short *)(menu + 0x14)
                    );
                }

                if (   (sdata->memcardAction == 1) // If you are saving data
                    && (sdata->boolMemcardDataValid != 0) // if the save is out-dated
                )
                {
                    // DAT_8008d878 + 0x340
                    // ...DATA ON MEMORY CARD IS OUT OF DATE...
                    DecalFont_DrawLine(
                        sdata->lngStrings[0x340],
                        0x100,
                        0xc3,
                        FONT_SMALL,
                        0xffff8000 | RED
                    );
                }
            }
        }
        else
        {
            sdata->data10_bbb[4] = 0;

            if (   (   (sdata->data10_bbb[1] == 0)
                    || ((   (   sdata->unk8008d964 == 0
                             || (sdata->data10_bbb[2] != 0))
                         || (sdata->data10_bbb[3] != 0))))
                || // timerSaveComplete
                   (sdata->data10_bbb[6] == 0)
            )
            {
                // mcScreenText
                iVar10 = (int)(short)uVar14 * 4;
                uVar19 = *(unsigned short *)(iVar10 + -0x7ff7a4fc); // 0x80085B04

                if (   ((int)(short)uVar14 == 9)
                    && (sdata->data10_bbb[0] == 0x40)
                )
                {
                    uVar19 = 0xffff;
                }
                iVar23 = (unsigned int)uVar19 << 0x10;
                if (   sdata->data10_bbb[1] != 0
                    && (iVar23 = (unsigned int)uVar19 << 0x10, sdata->unk8008d964 != 0)
                )
                {
                    uVar19 = 0xffff;
                    iVar23 = -0x10000;
                }
                if ((-1 < iVar23 >> 0x10) && (sdata->data10_bbb[3] == 0))
                {
                    if (   (iVar23 >> 0x10 == 0x10f)
                        && // If you are saving data
                           (sdata->memcardAction == 1)
                    )
                    {
                        // Displays this string
                        // WARNING:
                        // on the screen
                        uVar19 = 0x106;
                    }

                    if (   (   // if you are handling adventure data
                               (sdata->data10_bbb[0] != 0x30)
                            && (uVar14 == 9)
                           )
                        && // If the save is out-dated
                           (sdata->boolMemcardDataValid != 0)
                    )
                    {
                        // DAT_8008d878 + 0x340
                        // ...DATA ON MEMORY CARD IS OUT OF DATE...
                        DecalFont_DrawLine(
                            sdata->lngStrings[0x340],
                            0x100,
                            0xc3,
                            FONT_SMALL,
                            0xffff8000 | RED
                        );
                    }

                    // 0xea
                    // SAVING TO MEMORY CARD...

                    if (   // If you are deleting data
                           (sdata->memcardAction == 2)
                        && (uVar19 == 0xea)
                    )
                    {
                        // DELETING GAME SLOT...
                        uVar19 = 0xfc;
                    }
                    if (*(short *)(iVar10 + -0x7ff7a4fa) == 0)  // 0x80085B06
                    {
                        DecalFont_DrawLine(              // really just uVar19 * 4
                            *(char *)(((int)((unsigned int)uVar19 << 0x10) >> 0xe) + sdata->lngStrings),
                            0x108,
                            0x12,
                            FONT_BIG,
                            (unsigned int)text_color | 0xffff8000
                        );
                    }
                    else
                    {
                        uVar24 = (unsigned int)text_color;
                        iVar10 = 0;
                        char *stringPtr;

                        for (iVar23 = 0; iVar23 * 0x10000 >> 0x10 < 9; iVar23++)
                        {
                            iVar10 = iVar10 >> 0x10;
                            stringPtr = *(char *)(((short)uVar19 + iVar10) * 4 + sdata->lngStrings);
                            iVar8 = strlen(stringPtr);
                            if (iVar8 != 0)
                            {
                                iVar8 = 2;
                                if (iVar10 != 0) iVar8 = 4;
                                iVar12 = iVar10 * ((int)*(short *)(&(data.font_charPixHeight[0]) + iVar8) + 2);
                                iVar8 = iVar12 + 0x26;
                                if (0 < iVar10) iVar8 = iVar12 + 0x2e;

                                // Draw big string
                                short font = FONT_BIG;

                                uVar21 = uVar24 | 0xffff8000;

                                if ((short)iVar23 == 0)
                                {
                                    if ((sdata->frameCounter & 4) == 0)
                                    {
                                        uVar21 = 0xffff8000 | RED;
                                    }
                                }
                                else
                                {
                                    font = FONT_SMALL;
                                }

                                // Draw String
                                DecalFont_DrawLine(
                                    stringPtr,
                                    0x100,
                                    iVar8 * 0x10000 >> 0x10,
                                    font,
                                    uVar21
                                );
                            }
                            iVar10 = iVar23 * 0x10000;
                        }
                    }

                    // Draw 2D Menu rectangle background
                    RECTMENU_DrawInnerRect(
                        &(sdata->unk_BeforeTokenMenu[0]),
                        (int)*(short *)(menu + 0x14),
                        sdata->gGT->backBuffer->otMem.startPlusFour
                    );
                }
            }
            else
            {
                int str_flags = 0xffff8000 | WHITE;
                if ((sdata->frameCounter & 4) == 0) str_flags = 0xffff8000 | ORANGE;

                // DAT_8008d878 + 0x4f4
                // SAVE COMPLETED.
                DecalFont_DrawLine(
                    sdata->lngStrings[0x4f4],
                    0x108,
                    100,
                    FONT_BIG,
                    str_flags
                );
            }
        }
    }

    // timerSaveComplete
    sVar22 = sdata->data10_bbb[6];

    if (   (   sdata->data10_bbb[1] != 0
            && sdata->boolError != 0
            && (   sdata->unk8008d964 != 0
                || sdata->data10_bbb[2] != 0
                || sdata->data10_bbb[3] != 0
               )
           )
        && ((   sdata->data10_bbb[1] == 0
             || ((   (   sdata->unk8008d964 == 0
                      || sdata->data10_bbb[2] != 0
                      || sdata->data10_bbb[3] != 0
                     )
                  || (// timerSaveComplete
                      sVar22 = sdata->data10_bbb[6] - 1,
                      sdata->data10_bbb[6] == 0
                     )
                 ))
            )
           )
    )
    {
        // SelectProfile_InitAndDestroy
        //FUN_80048edc();
        void (*FUN_80048edc)() = 0x80048edc;
        (*FUN_80048edc)();

        // RefreshCard_StopMemcardAction
        //FUN_800471ac();
        void (*FUN_800471ac)() = 0x800471ac;
        (*FUN_800471ac)();

        // If you are handling Adventure Profiles
        if (sdata->data10_bbb[0] == 0x20)
        {
            if (   (sdata->data10_bbb[2] == 0)
                && (sdata->advProfileIndex = menu->rowSelected,
                    // If you are loading data
                    sdata->memcardAction == 0)
            )
            {
                GAMEPROG_AdvPercent(&(sdata->advProgress));

                // SelectProfile_QueueLoadHub_MenuProc,
                // open menu to load profile, while already on adv hub
                sdata->ptrDesiredMenu = &data.menuQueueLoadHub;

                // DAT_8008fbd2
                // holds Level ID of the hub that your adventure
                // profile was saved in

                // Level in Track Selection
                // The level you want to load is the level you
                // Get from the Adventure Profile that you are loading
                sdata->gGT->currLEV = sdata->advProgress.HubLevYouSavedOn;
                data.menuGreenLoadSave.rowSelected = 3;

                // timerSaveComplete
                sVar22 = sdata->data10_bbb[6];
            }
            else
            {
                // Change Menu to Green Save/Load screen
                sdata->ptrDesiredMenu = &data.menuGreenLoadSave;

                data.menuGreenLoadSave.rowSelected = 3;

                // timerSaveComplete
                sVar22 = sdata->data10_bbb[6];
            }
        }
        else
        {
            if (sdata->data10_bbb[0] < 0x21)
            {
                if (sdata->data10_bbb[0] == 0)
                {
                    if (sdata->data10_bbb[2] != 0)
                    {
                        // Change active Menu to OSK
                        sdata->ptrDesiredMenu = &data.menuSubmitName;

                        // SubmitName_RestoreName
                        //FUN_8004aa08(0);
                        void (*FUN_8004aa08)(short i) = 0x8004aa08;
                        (*FUN_8004aa08)(0);

                        return;
                    }
                    sdata->advProfileIndex = menu->rowSelected;

                    // Level in Track Selection = 0x1a
                    // This means the level you want to load is N Sanity Beach Adventure Hub
                    // Which means you're creating a new adventure profile
                    sdata->gGT->currLEV = N_SANITY_BEACH;

                    Garage_Leave();
                }
                else
                {
                    if (sdata->data10_bbb[0] != 0x10)
                    {
                        return;
                    }
                    if (sdata->data10_bbb[2] != 0)
                    {
                        // TransitionTo_MainMenu_Returning
                        //FUN_800b4334();
                        void (*FUN_800b4334)() = 0x800b4334;
                        (*FUN_800b4334)();

                        // MM_AdvNewLoad_GetMenuPtr
                        //iVar10 = FUN_800ad980();
                        int (*FUN_800ad980)() = 0x800ad980;
                        iVar10 = (*FUN_800ad980)();

                        *(unsigned int *)(iVar10 + 8) = *(unsigned int *)(iVar10 + 8) & 0xfffffffb;
                        return;
                    }

                    sdata->advProfileIndex = menu->rowSelected;

                    // play with the name you entered in OSK
                    strncpy(sdata->gGT->prevNameEntered, sdata->advProgress.name, 17);
                    strncpy(sdata->gGT->currNameEntered, sdata->advProgress.name, 17);
                    #if 0
                    sdata->gGT->prevNameEntered[0] = sdata->advProgress.name[0];
                    sdata->gGT->prevNameEntered[4] = sdata->advProgress.name[4];
                    sdata->gGT->prevNameEntered[8] = sdata->advProgress.name[8];
                    sdata->gGT->prevNameEntered[12] = sdata->advProgress.name[12];
                    sdata->gGT->prevNameEntered[16] = sdata->advProgress.name[16];
                    sdata->gGT->currNameEntered[0] = sdata->advProgress.name[0];
                    sdata->gGT->currNameEntered[4] = sdata->advProgress.name[4];
                    sdata->gGT->currNameEntered[8] = sdata->advProgress.name[8];
                    sdata->gGT->currNameEntered[12] = sdata->advProgress.name[12];
                    sdata->gGT->currNameEntered[16] = sdata->advProgress.name[16];
                    #endif

                    // Track to load is N Sanity Beach Adventure HUB
                    iVar10 = N_SANITY_BEACH;

                    // If your save profile has a Level ID of where
                    // you saved the profile (if you ever manually saved)
                    if (sdata->advProgress.HubLevYouSavedOn != 0)
                    {
                        // Set iVar10 to the levl ID of the adventure hub
                        // you last saved the profile in
                        iVar10 = sdata->advProgress.HubLevYouSavedOn;
                    }

                    // set the level you want to load to iVar10
                    sdata->gGT->currLEV = iVar10;
                }
            }
            else
            {
                if (sdata->data10_bbb[0] != 0x30)
                {
                    if (sdata->data10_bbb[0] != 0x40)
                    {
                        return;
                    }

                    SelectProfile_Destroy();

                    if (sdata->boolSaveCupProgress == 0)
                    {
                        sdata->gGT->gameModeEnd &= ~(NEW_NAME | NEW_HIGH_SCORE);
                    }
                    else
                    {
                        if (sdata->data10_bbb[2] != 0)
                        {
                            // Change active Menu to
                            // "Save Game?" "Yes/No"
                            sdata->ptrDesiredMenu = &data.menuSaveGame;
                            return;
                        }
                    }

                    // make Menu invisible
                    RECTMENU_Hide(menu);
                    return;
                }

                SelectProfile_Destroy();

                // if you are saving data
                if (sdata->memcardAction == 1)
                {
                    if (sdata->data10_bbb[2] != 0)
                    {
                        // Change active Menu to
                        // end of race menu with "Save Ghost" option
                        //sdata->ptrDesiredMenu = &DAT_800a0458;
                        sdata->ptrDesiredMenu = (void (*)(struct RectMenu*)) 0x800a0458;
                        return;
                    }

                    // Change active Menu to
                    // end of race menu without "Save Ghost" option
                    //sdata->ptrDesiredMenu = &DAT_800a04a4;
                    sdata->ptrDesiredMenu = (void (*)(struct RectMenu*)) 0x800a04a4;
                    return;
                }
                if (sdata->data10_bbb[2] != 0)
                {
                    // Erase ghost of previous race from RAM
                    //FUN_80028410();
                    void (*FUN_80028410)() = 0x80028410;
                    (*FUN_80028410)();

                    // MM_TrackSelect_GetMenuPtr
                    struct RectMenu* (*FUN_800b0eac)() = 0x800b0eac;
                    sdata->ptrDesiredMenu = FUN_800b0eac();

                    // MM_TrackSelect_Init
                    //FUN_800affd0();
                    void (*FUN_800affd0)() = 0x800affd0;
                    (*FUN_800affd0)();
                    return;
                }

                // Make P2 the character that is saved in the
                // header of the ghost that you will see in the race
                data.characterIDs[1] = sdata->ptrGhostTapePlaying->characterID;
            }

            // change active Menu to QueueLoadTrack
            #if 0
            sdata->ptrDesiredMenu = (undefined *)FUN_80043c04();
            #endif
            sdata->ptrDesiredMenu = &data.menuQueueLoadTrack;

            // timerSaveComplete
            sVar22 = sdata->data10_bbb[6];
        }
    }

    // timerSaveComplete
    sdata->data10_bbb[6] = sVar22;

    return;
}
