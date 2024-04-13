#include "application.h"
#include "config.h" // Include the configuration header

// Include the WebSocket library header.
// Make sure you have the correct library for WebSocket communication.
#include "WebSocketClient.h"

// Placeholder for the WebSocketClient and TCPClient instances.
WebSocketClient webSocketClient;
TCPClient tcpClient;

// Server details
char serverAddress[] = "your.django.server.com"; // The Django server's address
int port = 443; // The port, use 443 for WSS/HTTPS connections; still considering SSH
String path = "/ws/loaddata/"; // The WebSocket path on your server.

void setup() {
    // Initialize serial communication at a baud rate of 9600.
    Serial.begin(9600); // Debug Terminal

    Serial1.begin(9600); // For TX/RX communication with OpenScale

    // Connect to the Wi-Fi network (adjust with your credentials).
    WiFi.connect("yourSSID", "yourPassword");
    //Wait for a connection.
    while (WiFi.ready() == false) {
        delay(100);  // Wait for Wi-Fi connection
    }
    Serial.println("Connected to WiFi.");

    // Attempt to connect to the WebSocket server.
    webSocketClient.connect(tcpClient, serverAddress, port, path);
    if (webSocketClient.connected()) {
        Serial.println("Connected to the WebSocket Server!");
    } else {
        Serial.println("Failed to connect to the WebSocket Server.");
    }
}

void loop() {
    if (webSocketClient.connected()) {
        // Hypothetical method to check for and receive data.
        String incomingMessage = webSocketClient.receive();
        if (incomingMessage == "GET_WEIGHT") {
            float loadSensorData = readLoadSensorData();
            String dataToSend = "{\"message\": \"" + String(loadSensorData, 2) + "\"}";
            webSocketClient.send(dataToSend);
            Serial.println("Sent weight data: " + dataToSend);
        }
    } else {
        Serial.println("Disconnected. Attempting to reconnect...");
        webSocketClient.connect(tcpClient, serverAddress, port, path);
    }

    // Include a small delay to prevent overwhelming the loop with connection attempts.
    delay(1000);
}

float readAndSendWeightData() {
    if (Serial1.available() > 0) {
        String weightData = Serial1.readStringUntil('\n');
        if (!weightData.isEmpty()) {
            float weight = weightData.toFloat();
            if (weight > 0) {
                String dataToSend = "{\"weight\": \"" + String(weight, 2) + "\"}";
                webSocketClient.send(dataToSend);
                Serial.println("Sent weight data: " + dataToSend);
            }
        }
    }
}
