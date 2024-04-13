#ifndef WEBSOCKETTRANSMITTER_H
#define WEBSOCKETTRANSMITTER_H

#include "WebSocketClient.h"


class WebSocketTransmitter {
public:
    static void initialize(const char* serverAddress, int port, const char* path);
    static void update(float weight);
};

private:
static WebSocketClient webSocketClient;

#endif
