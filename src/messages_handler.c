#include <common.h>
#include "messages_handler.h"
#include "reward_enums.h"
#include "SelectProfile_AllProfiles_MenuProc.h"

#define EMPTY_MESSAGE1 " xxxxxx20characters1"
#define EMPTY_MESSAGE2 " xxxxxx20characters2"
#define EMPTY_MESSAGE3 " xxxxxx20characters3"
#define MSG_POS_X 130
#define MSG_POS_Y_LINE1 180
#define MSG_POS_Y_LINE2 190
#define MSG_POS_Y_LINE3 200
#define LOCAL_MSG_SIZE 20
#define LOCAL_MSG_BUF_SIZE 5

static char ap_message_buffer_line1[] = EMPTY_MESSAGE1;
static char ap_message_buffer_line2[] = EMPTY_MESSAGE2;
static char ap_message_buffer_line3[] = EMPTY_MESSAGE3;

enum MessageHandlerStates {
    WAITING_FOR_MESSAGE,
    DISPLAYING_MESSAGE_AP,
    DISPLAYING_MESSAGE_INTERNAL,
    INTER_MESSAGE_DELAY
};
enum MessageTimers {
    TIMER_VALUE_WAITING = 30,
    TIMER_INTER_MESSAGE_DELAY = 15,
    TIMER_DISPLAY_DURATION = 120
};

struct MessageMeta {
    char msg_type;
    char msg[LOCAL_MSG_SIZE + 1];
};

static struct MessageMeta local_msg_ringbuf[LOCAL_MSG_BUF_SIZE] = {0};
static short msg_ringbuf_write = 0;
static short msg_ringbuf_read = 0;

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

    randomizer_draw_credits();

    // Print the version of the randomizer into the bottom-left corner,
    // if we're not currently printing an unlock message
    if (msg_mode != DISPLAYING_MESSAGE_AP && msg_mode != DISPLAYING_MESSAGE_INTERNAL)
    {
        DecalFont_DrawLine("CTR Randomizer", 10, 190, FONT_SMALL, ORANGE);
        DecalFont_DrawLine("alpha 3            ", 10, 200, FONT_SMALL, ORANGE);
    }

    // Handle messsage handler states
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
    else if (msg_mode == DISPLAYING_MESSAGE_AP)
    {
        if (msg_timer > 0)
        {
            DecalFont_DrawLine(
                ap_message_buffer_line1,
                MSG_POS_X,
                MSG_POS_Y_LINE1,
                FONT_SMALL,
                ORANGE
            );
            DecalFont_DrawLine(
                ap_message_buffer_line2,
                MSG_POS_X,
                MSG_POS_Y_LINE2,
                FONT_SMALL,
                ORANGE
            );
            DecalFont_DrawLine(
                ap_message_buffer_line3,
                MSG_POS_X,
                MSG_POS_Y_LINE3,
                FONT_SMALL,
                ORANGE
            );
            msg_timer--;
        }
        else
        {
            ap_message_buffer_line1[0] = ' ';
            msg_mode = INTER_MESSAGE_DELAY;
            msg_timer = TIMER_INTER_MESSAGE_DELAY;
        }
    }
    else if (msg_mode == DISPLAYING_MESSAGE_INTERNAL)
    {
        if (msg_timer > 0)
        {
            switch (local_msg_ringbuf[msg_ringbuf_read].msg_type)
            {
                case MSGTYPE_UNLOCK:
                    DecalFont_DrawLine(
                        "Unlocked:",
                        MSG_POS_X,
                        MSG_POS_Y_LINE2,
                        FONT_SMALL,
                        ORANGE
                    );
                    break;

                case MSGTYPE_DBFETCH_ERROR:
                    DecalFont_DrawLine(
                        "DB-Fetch error:",
                        MSG_POS_X,
                        MSG_POS_Y_LINE2,
                        FONT_SMALL,
                        ORANGE
                    );
                    break;

                case MSGTYPE_SAPPHIRE_TIME:
                    DecalFont_DrawLine(
                        "Sapphire Time Reward",
                        MSG_POS_X,
                        MSG_POS_Y_LINE2,
                        FONT_SMALL,
                        TROPY_LIGHT_BLUE
                    );
                    break;

                case MSGTYPE_GOLD_TIME:
                    DecalFont_DrawLine(
                        "Gold Time Reward",
                        MSG_POS_X,
                        MSG_POS_Y_LINE2,
                        FONT_SMALL,
                        PAPU_YELLOW
                    );
                    break;

                case MSGTYPE_PLATINUM_TIME:
                    DecalFont_DrawLine(
                        "Platinum Time Reward",
                        MSG_POS_X,
                        MSG_POS_Y_LINE2,
                        FONT_SMALL,
                        SILVER
                    );
                    break;
            }
            DecalFont_DrawLine(
                local_msg_ringbuf[msg_ringbuf_read].msg,
                MSG_POS_X,
                MSG_POS_Y_LINE3,
                FONT_SMALL,
                PENTA_WHITE
            );
            msg_timer--;
        }
        else
        {
            local_msg_ringbuf[msg_ringbuf_read].msg_type = MSGTYPE_EMPTY;
            msg_ringbuf_read++;
            msg_ringbuf_read = msg_ringbuf_read % LOCAL_MSG_BUF_SIZE;

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
            // Check for new messages
            if (ap_message_buffer_line1[0] != ' ')
            {
                // Multiworld message is queued
                msg_mode = DISPLAYING_MESSAGE_AP;
                msg_timer = TIMER_DISPLAY_DURATION;
            }
            else if (local_msg_ringbuf[msg_ringbuf_read].msg_type != MSGTYPE_EMPTY)
            {
                // Internal message is queued
                msg_mode = DISPLAYING_MESSAGE_INTERNAL;
                msg_timer = TIMER_DISPLAY_DURATION;
            }
            else
            {
                msg_timer = TIMER_VALUE_WAITING;
            }
        }
    }
}

/* Add string pointed to by `msg_pointer` to the message handler ring buffer for
   internal messages. Silently overwrites messages if the number of queued
   messages exceeds the buffer size.
*/
void enqueue_message(
    char  msg_type,
    char *msg_pointer
)
{
    local_msg_ringbuf[msg_ringbuf_write].msg_type = msg_type;
    strncpy(local_msg_ringbuf[msg_ringbuf_write].msg, msg_pointer, LOCAL_MSG_SIZE);
    msg_ringbuf_write = ++msg_ringbuf_write % LOCAL_MSG_BUF_SIZE;
}

void enqueue_reward_message(
    char msg_type,
    int  reward
)
{
    char *msg_pointer;
    switch (reward)
    {
        case STATIC_TROPHY:
            msg_pointer = &("Trophy");
            break;
        case STATIC_RELIC | (RELIC_SAPPHIRE << 8):
            msg_pointer = &("Sapphire Relic");
            break;
        case STATIC_RELIC | (RELIC_GOLD << 8):
            msg_pointer = &("Gold Relic");
            break;
        case STATIC_RELIC | (RELIC_PLATINUM << 8):
            msg_pointer = &("Platinum Relic");
            break;
        case STATIC_TOKEN | (TOKEN_RED << 8):
            msg_pointer = &("Red CTR Token");
            break;
        case STATIC_TOKEN | (TOKEN_GREEN << 8):
            msg_pointer = &("Green CTR Token");
            break;
        case STATIC_TOKEN | (TOKEN_BLUE << 8):
            msg_pointer = &("Blue CTR Token");
            break;
        case STATIC_TOKEN | (TOKEN_YELLOW << 8):
            msg_pointer = &("Yellow CTR Token");
            break;
        case STATIC_TOKEN | (TOKEN_PURPLE << 8):
            msg_pointer = &("Purple CTR Token");
            break;
        case STATIC_KEY:
            msg_pointer = &("Key");
            break;
        case STATIC_GEM | (GEM_RED << 8):
            msg_pointer = &("Red Gem");
            break;
        case STATIC_GEM | (GEM_GREEN << 8):
            msg_pointer = &("Green Gem");
            break;
        case STATIC_GEM | (GEM_BLUE << 8):
            msg_pointer = &("Blue Gem");
            break;
        case STATIC_GEM | (GEM_YELLOW << 8):
            msg_pointer = &("Yellow Gem");
            break;
        case STATIC_GEM | (GEM_PURPLE << 8):
            msg_pointer = &("Purple Gem");
            break;
        default:
            msg_pointer = &("Unknown item");
            break;
    }
    enqueue_message(msg_type, msg_pointer);
}
