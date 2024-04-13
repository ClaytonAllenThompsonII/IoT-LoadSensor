#include "ScaleManager.h"
#include "MenuSystem.h"
#include "WebSocketTransmitter.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Initializing OpenScale...");
    Serial1.begin(9600); // For TX/RX communication with OpenScale

    
    MenuSystem::initialize();
    ScaleManager::initialize();
    WebSocketTransmitter::initialize();
}

void loop() {
    static unsigned long lastUpdateTime = 0;
    unsigned long currentMillis = millis();

    // Update Frequency control (update every 100 ms)
    if (currentMillis - lastUpdateTime > 100) {
        MenuSystem::update();
        ScaleManager::update();
        WebSocketTransmitter::update();
        lastUpdateTime = currentMillis;
    }
}