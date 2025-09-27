#include <headers/ESP8266WiFi.h> // necessary libraries for Wifi functionality
#include <headers/espnow.h> // necessary library 

uint8_t peer[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC}; // receiver MAC

void onSent(uint8_t *mac, uint8_t status){ /* optional: check status */ }

void setup() {

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != 0) return; // initializes the ESP-NOW protocol 
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); //  Defines this device as the sen
  esp_now_add_peer(peer, ESP_NOW_ROLE_SLAVE, 1, NULL, 0); //  add the receiver device
  esp_now_register_send_cb(onSent);  // sets up the callback for send status 
}

void loop() {
  const char msg[] = "ALERT:TDS_HIGH";     // tiny packet
  esp_now_send(peer, (uint8_t*)msg, sizeof(msg));
  delay(5000);
}
