#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266WebServer.h>
String myMac;
char msg[251];  // 250 + '\0'

bool hasDataToSend;
bool hasReceivedData;
String outgoingData = "";
String incomingData = "";

void onRecv(uint8_t* mac, uint8_t* data, uint8_t len);
void onSent(uint8_t* mac, uint8_t status);
void broadcastMessage(String message);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP-Now Sender Starting..");
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed"); return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  myMac = WiFi.macAddress()
  Serial.println("My MAC: " + myMac);

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onRecv);
  esp_now_register_send_cb(onSent);

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Ready! Type messages in Serial Monitor to broadcast");
  Serial.println("Commands:");
  Serial.println("  'hello' - Send greeting to all devices");
  Serial.println("  'ping' - Ping all devices");
  Serial.println("  Any text - Broadcast that message");
}

// Events occuring if message is sent or broadcasted
void onSent(uint8_t* mac, uint8_t status){
  
  Serial.print("Send status to ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }

  Serial.print(" - ");
  Serial.println(status == 0 ? "Success" : "Failed");

}

// Events occurring if message received
void onRecv(uint8_t* mac, uint8_t* data, uint8_t len){
  char buffer[len + 1];
  memcpy(buffer, incomingData, len);
  buffer[len] = '\0';
  
  Serial.print("Received: ");
  Serial.println(String(buffer));
  hasReceivedData = true;
}

// Broadcasts message to available NodeMCUs
void broadcastMessage(String message) {
  
  uint8_t* messageBytes = (uint8_t*)message.c_str();
  int messageLength = message.length();
  int result = esp_now_send(broadcastAddress, messageBytes, messageLength);
  Serial.println((result == 0 ? "✓ Broadcast sent: " : "✗ Broadcast failed: ") + message);
}

void loop() {

  if (Serial.available()) {          // checks data if in arduno
      String sensorData = Serial.readString();
      broadcastMessage(sensorData);
    }

} 