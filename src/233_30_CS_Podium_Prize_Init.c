#include <common.h>
/* START RANDOMIZER */
#include "CTRRandomizer_database.h"
#include "reward_enums.h"
#include "CTRRandomizer_outsourcing.h"
/* END RANDOMIZER */

#define gte_ldVXY0(r0) __asm__ volatile("mtc2   %0, $0" \
                                        :               \
                                        : "r"(r0))
#define gte_ldVZ0(r0) __asm__ volatile("mtc2   %0, $1" \
                                       :               \
                                       : "r"(r0))

#define read_mt(r0, r1, r2) __asm__ volatile( \
    "mfc2   %0, $25;"                         \
    "mfc2   %1, $26;"                         \
    "mfc2   %2, $27;"                         \
    :                                         \
    : "r"(r0), "r"(r1), "r"(r2))

#define UI_HUD_ELEM_NUMRELIC 0xE
#define UI_HUD_ELEM_NUMKEY 0xF
#define UI_HUD_ELEM_NUMTROPHY 0x10

void CS_Podium_Prize_ThTick1(struct Thread *t);
void CS_Podium_Prize_ThDestroy(struct Thread *t);

void CS_Podium_Prize_Init(int prizeModel, char* prizeName, short *posOnScreen)
{
    u_int relicColor;
    u_int prizeFlag;
    int tx;
    int ty;
    int tz;

    short *prize;

    struct GameTracker *gGT = sdata->gGT;

    /* START RANDOMIZER */
    int local_prizeModel = GET_CLEAN_REWARD(prizeModel);
    int prizeColor = GET_REWARD_COLOR(prizeModel);
    /* END RANDOMIZER */

    // create thread, get instance
    // 0x200 flag = MediumStackPool
    // 0xd = "other" thread bucket
    struct Instance *inst = INSTANCE_BirthWithThread(
        local_prizeModel,
        prizeName,
        MEDIUM,
        OTHER,
        CS_Podium_Prize_ThTick1,
        0x2c,
        0
    );

    if (inst == NULL)
    {
        if (OVR_233.cutsceneState < 1) OVR_233.cutsceneState = 1;

        // driver now can move
        gGT->gameMode2 &= ~(VEH_FREEZE_PODIUM);
        return;
    }

    // set scale (x, y, z)
    inst->scale[0] = 0x2000;
    inst->scale[1] = 0x2000;
    inst->scale[2] = 0x2000;

    // make invisible
    inst->flags |= HIDE_MODEL;

    // get object from thread
    prize = inst->thread->object;

    inst->thread->funcThDestroy = CS_Podium_Prize_ThDestroy;

    prize[0x11] = 0x40;
    prize[0x12] = 0x200;

    prize[4] = 0;
    prize[5] = 0;
    prize[6] = 0;
#if 0
    gte_ldVXY0(0);
    gte_ldVZ0(0x40);
    gte_llv0();

    read_mt(tx, ty, tz);

    // pos x, y and z
    prize[0] = posOnScreen[0] + (short)tx;
    prize[1] = posOnScreen[1] + (short)ty + 0x1c0;
    prize[2] = posOnScreen[2] + (short)tz;
#endif
    prize[0] = posOnScreen[0];
    prize[1] = posOnScreen[1] + 0x1c0;
    prize[2] = posOnScreen[2];
    prize[10] = -0x200;

    struct UiElement2D (*ptrHudData)[] = data.hudStructPtr[0];

    // specular lighting
    inst->flags |= USE_SPECULAR_LIGHT;

    switch (local_prizeModel)
    {
        // if reward is [empty], used for Oxide Podium
        case STATIC_BIG1:
            // make invisible
            inst->flags |= HIDE_MODEL;
            // not specular lighting
            inst->flags &= ~USE_SPECULAR_LIGHT;
            goto GEMS_OR_TOKEN_OR_NOTHING;

        case STATIC_TOKEN:
            prizeColor--;
            /* Intentional fall-through */

        // if reward is gem
        case STATIC_GEM:
            // get color of the gem based off the cup ID
            if (prizeColor < 5)
            {
                short *gemColor = &data.AdvCups[prizeColor].color;

                inst->colorRGBA = (gemColor[0] << 20 | gemColor[1] << 12 | gemColor[2] << 4);
            }
            else
            {
                // Gem with prizeColor == 5: our tag for multiworld-item
                // Make gem bright white
                inst->colorRGBA = 0xe0e0e00;
            }

            prize[12] = 0x5d3;
            prize[13] = 0x718;
            prize[14] = 0x590;
            prize[15] = 0x609;

        default:
            GEMS_OR_TOKEN_OR_NOTHING:
            prize[8] = 0x100;
            prize[9] = 0x6c;
            return;

        // if reward is relic
        case STATIC_RELIC:
            prize[8] = (*ptrHudData)[UI_HUD_ELEM_NUMRELIC].x;
            prize[9] = (*ptrHudData)[UI_HUD_ELEM_NUMRELIC].y - 60;

            // set color of relic
            inst->colorRGBA = (prizeColor == RELIC_SAPPHIRE)
                ? 0x20a5ff0 // relic blue
                : (prizeColor == RELIC_GOLD)
                    ? 0xd8d2090 // relic gold
                    : 0xffede90 // relic platinum
            ;

            // pack relic color and a 75 frames lock timer together and pass to
            // CTRRandomizer_outsourcing > randomizer_get_num_relic()
            hud_relic_color_lock = prizeColor | (75 << 4);

            prize[12] = 0x2ab;
            prize[13] = 0x436;
            prize[14] = 0x1eb;
            prize[15] = 0x670;

            prizeFlag = INC_RELIC;
            break;

        // if reward is trophy
        case STATIC_TROPHY:
            prize[8] = (*ptrHudData)[UI_HUD_ELEM_NUMTROPHY].x;
            prize[9] = (*ptrHudData)[UI_HUD_ELEM_NUMTROPHY].y - 60;
            prize[10] = -200;

            // set scale (x, y, z)
            inst->scale[0] = 0x4000;
            inst->scale[1] = 0x4000;
            inst->scale[2] = 0x4000;

            // not specular lighting
            inst->flags &= ~USE_SPECULAR_LIGHT;

            prizeFlag = INC_TROPHY;
            break;

        // if reward is key
        case STATIC_KEY:
            prize[8] = (*ptrHudData)[UI_HUD_ELEM_NUMKEY].x;
            prize[9] = (*ptrHudData)[UI_HUD_ELEM_NUMKEY].y - 60;

            inst->colorRGBA = 0xdca6000;
            prize[12] = 0x1d9;
            prize[13] = 0x5db;
            prize[14] = 0x2da;
            prize[15] = 0x54b;

            prizeFlag = INC_KEY;
    }

    gGT->gameMode2 |= prizeFlag;
}
