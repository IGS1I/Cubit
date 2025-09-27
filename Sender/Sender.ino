#include <ESP8266WiFi.h>
#include <espnow.h>

// IMPORTANT: Replace with your receiver's actual MAC address
//sender
uint8_t peer[] = {0x48, 0x55, 0x19, 0xed, 0x33, 0x11}; // Your receiver's MAC
 
void onSent(uint8_t *mac, uint8_t status) {
  Serial.print("Send status to ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" - ");
  Serial.println(status == 0 ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP-NOW Sender Starting...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  // Add peer device (ESP8266 syntax)
  if (esp_now_add_peer(peer, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_send_cb(onSent); // sets up the callback for send status

  // ESP8266 syntax for adding peer (much simpler!)
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(peer, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_register_send_cb(onSent);
  
  Serial.println("ESP-NOW Sender initialized successfully");
}

void loop() {
  const char msg[] = "ALERT:TDS_HIGH";
  Serial.print("Sending: ");
  Serial.println(msg);

  uint8_t result = esp_now_send(peer, (uint8_t*)msg, sizeof(msg) - 1); // -1 to exclude null terminator
  if (result != 0) {
    Serial.println("Send failed immediately");
  }

  delay(5000);
}