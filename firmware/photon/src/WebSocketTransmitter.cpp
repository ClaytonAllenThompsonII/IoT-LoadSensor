#include "WebSocketTransmitter.h"
#include "ScaleManager.h"
#include "WebSocketClient.h"

WebSocketClient webSocketClient;

void WebSocketTransmitter::initialize() {
    // Setup WebSocket connection
}

void WebSocketTransmitter::update() {
    // Send data to WebSocket server
    float weight = ScaleManager::getWeight();
    webSocketClient.send(weight);
}
