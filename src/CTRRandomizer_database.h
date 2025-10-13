#ifndef CTRRANDOMIZER_DATABASE_H
#define CTRRANDOMIZER_DATABASE_H

#include <common.h>

#define GET_GEMANDTOKEN_COLOR(x) ((x >> 8) & 0xF)
#define GET_CLEAN_REWARD(x) (x & 0xFF)

/* Key type defines via prefixes */
enum DB_Prefixes {
    DB_PREFIX_LEVELIDS = 0xA0000000,
    DB_PREFIX_REWARDS  = 0xA1000000,
    DB_PREFIX_SETTINGS = 0xAF000000
};

/* DB fetch results */
enum DbFetchResult {
    DB_VALUE_OK,
    DB_VALUE_NOTFOUND
};

enum SettingsIds {
    SETTING_RELIC_DIFFICULTY = 0,
    SETTING_RELIC_NEEDS_PERFECT
};
enum RelicDifficulty {
    RELICDIFF_SAPPHIRE = 0,
    RELICDIFF_GOLD,
    RELICDIFF_PLATINUM
};

int database_fetch(
    int db_key,
    int *fetch_result
);

#endif