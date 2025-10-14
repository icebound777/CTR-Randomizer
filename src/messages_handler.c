#include <common.h>


/* Print the version of the randomizer into the bottom-left corner */
void print_version()
{
    if (!(sdata->gGT->gameMode1 & LOADING))
    {
        DecalFont_DrawLine("CTR Randomizer", 10, 190, FONT_SMALL, ORANGE);
        DecalFont_DrawLine("pre-alpha          ", 10, 200, FONT_SMALL, ORANGE);
    }
}
