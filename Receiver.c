#include <headers/ESP8266WiFi.h>
#include <headers/espnow.h>

void onRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
// mac: address of the sender, data : actual message data, len: length of message
  // handle message (e.g., update LCD, re-broadcast)
} // called automatically whenever a message is received. 

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != 0) return;
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onRecv);
}

void loop() { /* idle */ }
