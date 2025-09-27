#include <headers/ESP8266WiFi.h>
#include <headers/espnow.h>

// Callback function for receiving data
void onReceive(uint8_t *mac, uint8_t *data, uint8_t len) {
  Serial.print("Received from MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  Serial.print("Message: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();
  
  Serial.print("Length: ");
  Serial.println(len);
  Serial.println("---");
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP-NOW Receiver Starting...");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
 
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  // Set device role as slave (receiver)
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  
  // Register callback for receiving data
  esp_now_register_recv_cb(onReceive);
  
  Serial.println("ESP-NOW Receiver initialized successfully");
  Serial.println("Waiting for messages...");
}

void loop() {
  // Receiver doesn't need to do anything in loop
  // All work is done in the onReceive callback
  delay(1000);
}
