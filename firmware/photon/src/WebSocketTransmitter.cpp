#include "WebSocketTransmitter.h"
#include "ScaleManager.h"
#include "WebSocketClient.h"

WebSocketClient webSocketClient;

void WebSocketTransmitter::initialize(const char* serverAddress, int port, const char* path) {
    // Setup WebSocket connection
    if (webSocketClient.connect(serverAddress, port, path)) {
        Serial.println("WebSocket connected successfully.");
    } else {
        Serial.println("WebSocket connection failed.");
    }
}

void WebSocketTransmitter::update() {
    // Send data to WebSocket server
    float weight = ScaleManager::getWeight();
    webSocketClient.send(weight);
}
