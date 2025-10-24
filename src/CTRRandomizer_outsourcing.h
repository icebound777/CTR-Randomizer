#ifndef CTRRANDOMIZER_INIT_PROFILE_H
#define CTRRANDOMIZER_INIT_PROFILE_H

void randomizer_set_profile_defaults(struct AdvProgress* adv);
int randomizer_garage_tick_get_bossID(
    char  levelID,
    char  hubID
);
int randomizer_get_num_relic(void);
void randomizer_store_relic_time(int time);

extern struct Instance *advhud_relic;

#endif
