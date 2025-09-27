#include <ESP8266WiFi.h>
#include <espnow.h>

// Replace with actual receiver MAC address
uint8_t peer[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC}; 

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
  
  // Set device role as controller (sender)
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Add peer device
  if (esp_now_add_peer(peer, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  }
  
  // Register callback for send status
  esp_now_register_send_cb(onSent);
  
  Serial.println("ESP-NOW initialized successfully");
}

void loop() {
  const char msg[] = "ALERT:TDS_HIGH";
  Serial.print("Sending: ");
  Serial.println(msg);
  
  uint8_t result = esp_now_send(peer, (uint8_t*)msg, sizeof(msg));
  if (result != 0) {
    Serial.println("Send failed");
  }
  
  delay(5000);
}