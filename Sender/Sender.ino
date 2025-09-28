#include <ESP8266WiFi.h>
#include <espnow.h>

void onReceive(uint8_t* mac, uint8_t* data, uint8_t len) {
  Serial.printf("Got message (%d bytes): %.*s\n", len, len, data);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onReceive);

  Serial.println("Write an error message:\n");
}

void loop() {

  String message = Serial.readString();
  if (message == "") {
    return;
  }
  Serial.print(message);
  sendPacket(message);
}

void sendPacket(String message){
  String packetStr = message;   // concatenate
  const char* packet = message.c_str();   // get const char*
  uint8_t bcast[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  esp_now_send(bcast, (uint8_t*)packet, strlen(packet));
}
