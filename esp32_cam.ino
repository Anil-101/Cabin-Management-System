#include <WiFi.h>

const char* ssid = "Everything";       // Change to your WiFi
const char* password = "12345678";
const int relayLight = 4;            // Light relay pin
const int relayFan = 5;              // Fan relay pin

WiFiClient client;
const char* serverIP = "192.168.135.84";  // Change to your server IP
const int serverPort = 12345;
String receivedData = "";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    pinMode(relayLight, OUTPUT);
    pinMode(relayFan, OUTPUT);

    digitalWrite(relayLight, HIGH);  // Start with lights OFF
    digitalWrite(relayFan, HIGH);    // Start with fan OFF

    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server");
    } else {
        Serial.println("Failed to connect to server");
    }
}

void loop() {
    if (client.connected()) {
        while (client.available()) {
            receivedData = client.readStringUntil('\n');
            receivedData.trim();
            Serial.println("Received: " + receivedData);

            // Light and Fan Control
            controlLight(receivedData);
            controlFan(receivedData);
        }
    } else {
        Serial.println("Reconnecting to server...");
        if (client.connect(serverIP, serverPort)) {
            Serial.println("Reconnected to server");
        }
    }
}

// === Light Control Function ===
void controlLight(String command) {
    if (command.indexOf("Light:ON") >= 0) {
        digitalWrite(relayLight, LOW);  // Turn ON light
        Serial.println("Light turned ON");
    } else if (command.indexOf("Light:OFF") >= 0) {
        digitalWrite(relayLight, HIGH); // Turn OFF light
        Serial.println("Light turned OFF");
    }
}

// === Fan Control Function ===
void controlFan(String command) {
    if (command.indexOf("Fan:ON") >= 0) {
        digitalWrite(relayFan, LOW);  // Turn ON fan
        Serial.println("Fan turned ON");
    } else if (command.indexOf("Fan:OFF") >= 0) {
        digitalWrite(relayFan, HIGH); // Turn OFF fan
        Serial.println("Fan turned OFF");
    }
}
