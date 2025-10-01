#ifndef CTRRANDOMIZER_DATABASE_H
#define CTRRANDOMIZER_DATABASE_H

#include <common.h>

/* Key type defines via prefixes */
enum DbPrefixes {
    DB_PREFIX_LEVELIDS = 0xA0000000,
    DB_PREFIX_UNLOCK_ITEMTYPE = 0xA1000000,
    DB_PREFIX_UNLOCK_ITEMCOUNT = 0xA2000000,
};

/* DB fetch results */
enum DbFetchResult {
    DB_VALUE_OK,
    DB_VALUE_NOTFOUND
};

int database_fetch(
    int db_key,
    int *fetch_result
);

#endif