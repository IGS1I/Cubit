#include <ESP8266WiFi.h>
#include <espnow.h>

void onReceive(uint8_t* mac, uint8_t* data, uint8_t len) {
  Serial.printf("%.*s\n", len, data);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onReceive);
}

void loop() {
}
