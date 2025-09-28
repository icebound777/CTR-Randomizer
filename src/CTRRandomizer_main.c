#include <common.h>

#define RANDO_CHECK_INTERVAL_FRAMES     2000
#define RANDO_ITEM_RECEIVED_QUEUE_SIZE  8
#define RANDO_SHOW_ITEM_RECEIVED_FRAMES 90

void CTRRandomizer_main(void)
{
    struct GameTracker *gGT;

    static int frame_counter_chk_queue = RANDO_CHECK_INTERVAL_FRAMES;
    static int item_received_counter = 0;
    static int item_recv_queue[RANDO_ITEM_RECEIVED_QUEUE_SIZE] = {0};
    int new_item;

    static int frame_counter_show_msg = 0;

    /* Don't process during loading screens */
    gGT = sdata->gGT;
    if ((gGT->gameMode1 & LOADING)) return;

    if (frame_counter_chk_queue > 0)
    {
        frame_counter_chk_queue--;
    }
    else if (frame_counter_chk_queue == 0)
    {
        /* TESTING: */ item_recv_queue[0] = 1;
        new_item = item_recv_queue[0];
        if (new_item != 0)
        {
            /* shift item received queue */
            for (int i = 0; i < RANDO_ITEM_RECEIVED_QUEUE_SIZE - 1; i++)
            {
                item_recv_queue[i] = item_recv_queue[i + 1];
            }
            item_recv_queue[RANDO_ITEM_RECEIVED_QUEUE_SIZE - 1] = 0;

            /* Increment item received counter */
            item_received_counter++;

            /* Add item to save file */
            /* TODO */

            /* Draw associated message */
            frame_counter_show_msg = RANDO_SHOW_ITEM_RECEIVED_FRAMES;
        }

        frame_counter_chk_queue = RANDO_CHECK_INTERVAL_FRAMES;
    }

    if (frame_counter_show_msg > 0)
    {
        DecalFont_DrawLine("Received:", 10, 200 - 10, FONT_SMALL, ORANGE);
        DecalFont_DrawLine("Boss Key 1", 10, 200, FONT_SMALL, ORANGE);
        frame_counter_show_msg--;
        frame_counter_chk_queue = RANDO_CHECK_INTERVAL_FRAMES;
        gGT->currAdvProfile.numKeys = 1;
    }
}
