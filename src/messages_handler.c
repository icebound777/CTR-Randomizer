#include <common.h>

#define EMPTY_MESSAGE " xxxxxxx20characters"

static char ap_message_buffer_line1[] = EMPTY_MESSAGE;
static char ap_message_buffer_line2[] = EMPTY_MESSAGE;
static char ap_message_buffer_line3[] = EMPTY_MESSAGE;

enum MessageHandlerStates {
    WAITING_FOR_MESSAGE,
    DISPLAYING_MESSAGE,
    INTER_MESSAGE_DELAY
};
enum MessageTimers {
    TIMER_VALUE_WAITING = 30,
    TIMER_INTER_MESSAGE_DELAY = 15,
    TIMER_DISPLAY_DURATION = 120
};

/* Handles all messages the randomizer wants to print */
void messages_handler()
{
    static int msg_mode = WAITING_FOR_MESSAGE;
    static int msg_timer = TIMER_VALUE_WAITING;

    if ((sdata->gGT->gameMode1 & LOADING))
    {
        msg_timer = TIMER_DISPLAY_DURATION;
        return;
    }

    // Print the version of the randomizer into the bottom-left corner,
    // if we're not currently printing an unlock message
    if (msg_mode != DISPLAYING_MESSAGE)
    {
        DecalFont_DrawLine("CTR Randomizer", 10, 190, FONT_SMALL, ORANGE);
        DecalFont_DrawLine("pre-alpha          ", 10, 200, FONT_SMALL, ORANGE);
    }

    // Handle messsage queue states
    if (msg_mode == INTER_MESSAGE_DELAY)
    {
        if (msg_timer > 0)
        {
            msg_timer--;
        }
        else
        {
            msg_mode = WAITING_FOR_MESSAGE;
            msg_timer = TIMER_VALUE_WAITING;
        }
    }
    else if (msg_mode == DISPLAYING_MESSAGE)
    {
        if (msg_timer > 0)
        {
            DecalFont_DrawLine(ap_message_buffer_line1, 130, 180, FONT_SMALL, ORANGE);
            DecalFont_DrawLine(ap_message_buffer_line2, 130, 190, FONT_SMALL, ORANGE);
            DecalFont_DrawLine(ap_message_buffer_line3, 130, 200, FONT_SMALL, ORANGE);
            msg_timer--;
        }
        else
        {
            ap_message_buffer_line1[0] = ' ';
            ap_message_buffer_line2[0] = ' ';
            ap_message_buffer_line3[0] = ' ';
            msg_mode = INTER_MESSAGE_DELAY;
            msg_timer = TIMER_INTER_MESSAGE_DELAY;
        }
    }
    else // (msg_mode == MODE_WAITING_FOR_MESSAGE)
    {
        if (msg_timer > 0)
        {
            msg_timer--;
        }
        else
        {
            // Check for new message
            if (ap_message_buffer_line1[0] == ' ')
            {
                msg_timer = TIMER_VALUE_WAITING;
            }
            else
            {
                // New message: Display it
                msg_mode = DISPLAYING_MESSAGE;
                msg_timer = TIMER_DISPLAY_DURATION;
            }
        }
    }
}
