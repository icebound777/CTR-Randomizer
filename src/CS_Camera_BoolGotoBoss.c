#include <common.h>

u_char CS_Camera_BoolGotoBoss(void)
{
    return 0;

/* RANDOMIZER - We never want boss cutscenes ever*/
#if 0
    struct GameTracker* gGT = sdata->gGT;

    if (   gGT->podiumRewardID == STATIC_RELIC
        && gGT->currAdvProfile.numRelics >= 18
    )
    {
        // If just got 18th relic
        // skip check to BeatOxide2 (obvious not beaten)
        return 1;
    }

    // If just unlocked Key
    if (gGT->podiumRewardID == STATIC_KEY) return 1;

    struct Instance* inst = gGT->drivers[0]->instSelf;
    short *posCoords = gGT->level1->ptrSpawnType2_PosRot[1].posCoords;

    // TRUE if TeleportSelf did NOT spawn on podium (goto boss door)
    return (
        inst->matrix.t[0] != posCoords[0]
     && inst->matrix.t[2] != posCoords[2]
    );
#endif
}
