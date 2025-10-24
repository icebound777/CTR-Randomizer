#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

enum MsgTypes {
    MSGTYPE_EMPTY,
    MSGTYPE_DBFETCH_ERROR,
    MSGTYPE_UNLOCK
};

void enqueue_message(
    int   msg_type,
    char *msg_pointer
);

#endif