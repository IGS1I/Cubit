#include <ESP8266WiFi.h> // necessary libraries for Wifi functionality
#include <espnow.h> // necessary library 

// IMPORTANT: Replace with your receiver's actual MAC address (check receiver serial output)
uint8_t peer[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC}; // receiver MAC - UPDATE THIS!

void onSent(uint8_t *mac, uint8_t status) {
  Serial.print("Send status: ");
  Serial.println(status == 0 ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP-NOW Sender Starting...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  // Add peer device
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, peer, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_send_cb(onSent); // sets up the callback for send status
  Serial.println("ESP-NOW Sender initialized successfully");
}

void loop() {
  const char msg[] = "ALERT:TDS_HIGH";     // tiny packet
  Serial.print("Sending: ");
  Serial.println(msg);
  
  uint8_t result = esp_now_send(peer, (uint8_t*)msg, sizeof(msg));
  // peer is the mac address of the receiver, msg is raw bytes of the message, sizeof(msg) is the length of the message
  if (result != 0) {
    Serial.println("Send failed immediately");
  }
  
  delay(5000);
}
