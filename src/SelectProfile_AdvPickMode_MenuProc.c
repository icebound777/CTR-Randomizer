#include <common.h>

enum SaveLoadMenuRow {
    ROW_LOAD = 0,
    ROW_SAVE = 1,
    ROW_DELETE = 2,
    ROW_EXIT = 3
};

void DECOMP_SelectProfile_AdvPickMode_MenuProc(struct RectMenu* menu)
{
    // if have not chosen any option
    // if you see current profile, and load/save/delete/exit rows
    if (menu->unk1e != 0)
    {
        SelectProfile_Init(menu->drawStyle);
        SelectProfile_DrawAdvProfile(&sdata->advProgress, 0x92, 0x32, 0, 0, 0x10);
        return;
    }

    // only here if you chose one of the rows (save/load/delete/exit)

    // get value of row selected
    short row = menu->rowSelected;

    if (row < ROW_EXIT && row >= ROW_LOAD)
    {
        // Set Load/Save to "Green" mode, remembering the last selected row
        SelectProfile_ToggleMode((row | 0x20));

        // Set Menu to a menu box that draws four adv profiles
        sdata->ptrDesiredMenu = &data.menuFourAdvProfiles;
        return;
    }

    if (row == ROW_EXIT)
    {
        // make Menu invisible
        RECTMENU_Hide(menu);

        // SelectProfile_Destroy
        SelectProfile_Destroy();
    }
}
