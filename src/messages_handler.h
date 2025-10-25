#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

enum MsgTypes {
    MSGTYPE_EMPTY,
    MSGTYPE_DBFETCH_ERROR,
    MSGTYPE_UNLOCK,
    MSGTYPE_SAPPHIRE_TIME,
    MSGTYPE_GOLD_TIME,
    MSGTYPE_PLATINUM_TIME
};

void enqueue_message(
    int   msg_type,
    char *msg_pointer
);
void enqueue_reward_message(
    int  msg_type,
    int  reward
);

#endif