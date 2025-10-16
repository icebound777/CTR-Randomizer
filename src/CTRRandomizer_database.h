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
    SETTING_RELIC_NEEDS_PERFECT,
    SETTING_BOSS_GARAGE_OPENING,
    SETTING_QOL_SKIP_MASKHINTS,
    SETTING_QOL_SKIP_PODIUM
};
enum RelicDifficulty {
    RELICDIFF_SAPPHIRE = 0,
    RELICDIFF_GOLD,
    RELICDIFF_PLATINUM
};
enum BossGarageOpening {
    GARAGE_OPENING_VANILLA_WARPPADS = 0,
    GARAGE_OPENING_CURRENTHUB_WARPPADS,
    GARAGE_OPENING_TROPHIES
};

int database_fetch(
    int db_key,
    int *fetch_result
);

#endif