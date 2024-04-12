#include "ScaleManager.h"
#include "MenuSystem.h"
#include "WebSocketTransmitter.h"

void setup() {
    Serial.begin(9600);
    MenuSystem::initialize();
    ScaleManager::initialize();
    WebSocketTransmitter::initialize();
}

void loop() {
    MenuSystem::update();
    ScaleManager::update();
    WebSocketTransmitter::update();
}