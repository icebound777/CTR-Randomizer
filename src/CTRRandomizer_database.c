#include <common.h>
#include "CTRRandomizer_database.h"


static int rando_database[] = {
    DB_PREFIX_LEVELIDS | CRASH_COVE,     CRASH_COVE,
    DB_PREFIX_LEVELIDS | ROO_TUBES,      ROO_TUBES,
    DB_PREFIX_LEVELIDS | TIGER_TEMPLE,   TIGER_TEMPLE,
    DB_PREFIX_LEVELIDS | COCO_PARK,      COCO_PARK,
    DB_PREFIX_LEVELIDS | MYSTERY_CAVES,  MYSTERY_CAVES,
    DB_PREFIX_LEVELIDS | BLIZZARD_BLUFF, BLIZZARD_BLUFF,
    DB_PREFIX_LEVELIDS | SEWER_SPEEDWAY, SEWER_SPEEDWAY,
    DB_PREFIX_LEVELIDS | DINGO_CANYON,   DINGO_CANYON,
    DB_PREFIX_LEVELIDS | PAPU_PYRAMID,   PAPU_PYRAMID,
    DB_PREFIX_LEVELIDS | DRAGON_MINES,   DRAGON_MINES,
    DB_PREFIX_LEVELIDS | POLAR_PASS,     POLAR_PASS,
    DB_PREFIX_LEVELIDS | CORTEX_CASTLE,  CORTEX_CASTLE,
    DB_PREFIX_LEVELIDS | TINY_ARENA,     TINY_ARENA,
    DB_PREFIX_LEVELIDS | HOT_AIR_SKYWAY, HOT_AIR_SKYWAY,
    DB_PREFIX_LEVELIDS | N_GIN_LABS,     N_GIN_LABS,
    DB_PREFIX_LEVELIDS | OXIDE_STATION,  OXIDE_STATION,
    DB_PREFIX_LEVELIDS | SLIDE_COLISEUM, SLIDE_COLISEUM,
    DB_PREFIX_LEVELIDS | TURBO_TRACK,    TURBO_TRACK,
    -1
};


/*
Fetch a value from the randomizer database using the given key.
If the value cannot be found, return the key as value and set the
fetch result to not_found.
*/
int database_fetch(
    int db_key,
    int *fetch_result
)
{
    for (int i = 0; rando_database[i] != -1; i+=2)
    {
        if (rando_database[i] == db_key)
        {
            *fetch_result = DB_VALUE_OK;
            return rando_database[i + 1];
        }
    }

    *fetch_result = DB_VALUE_NOTFOUND;
    return db_key;
}
