#include <common.h>

/* Handles all messages the randomizer wants to print */
void messages_handler()
{
    // Print the version of the randomizer into the bottom-left corner
    if (!(sdata->gGT->gameMode1 & LOADING))
    {
        DecalFont_DrawLine("CTR Randomizer", 10, 190, FONT_SMALL, ORANGE);
        DecalFont_DrawLine("pre-alpha          ", 10, 200, FONT_SMALL, ORANGE);
    }
}
