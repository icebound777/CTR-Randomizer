#ifndef CTRRANDOMIZER_ITEM_UNLOCKS_H
#define CTRRANDOMIZER_ITEM_UNLOCKS_H

void handle_item_unlocks(
    struct AdvProgress  *adv,
    int                 bitIndex,
    int                 item_type,
    int                 token_type
);

enum TokenAndGemColors {
    GEM_RED = 0,
    GEM_GREEN,
    GEM_BLUE,
    GEM_YELLOW,
    GEM_PURPLE,
    TOKEN_RED,
    TOKEN_GREEN,
    TOKEN_BLUE,
    TOKEN_YELLOW,
    TOKEN_PURPLE,
    TOKEN_NONE
};

#endif