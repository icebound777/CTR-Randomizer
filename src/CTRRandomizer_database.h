#ifndef CTRRANDOMIZER_DATABASE_H
#define CTRRANDOMIZER_DATABASE_H

#include <common.h>

#define GET_REWARD_COLOR(x) ((x >> 8) & 0xF)
#define GET_CLEAN_REWARD(x) (x & 0xFF)
#define GET_REQUIREMENT_TYPE(x)  (x & 0xFF)
#define GET_REQUIREMENT_COLOR(x) ((x >> 8) & 0xF)
#define GET_REQUIREMENT_COUNT(x) ((x >> 12) & 0xF)

/* Key type defines via prefixes */
enum DB_Prefixes {
    DB_PREFIX_LEVELIDS        = 0xA000,
    DB_PREFIX_REWARDS         = 0xA100,
    DB_PREFIX_WARPPADUNLOCK_1 = 0xA200,
    DB_PREFIX_WARPPADUNLOCK_2 = 0xA300,
    DB_PREFIX_SETTINGS        = 0xAF00
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
    SETTING_QOL_SKIP_PODIUM,
    SETTING_QOL_SKIP_CONGRATS,
    SETTING_OXIDE_REQUIRED_RELICS
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

unsigned short database_fetch(
    unsigned int db_key,
    int *fetch_result
);

#endif