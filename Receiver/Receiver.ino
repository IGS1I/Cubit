#include <ESP8266WiFi.h>
#include <espnow.h>

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
  delay(5000);  // Add this delay!
  Serial.println("ESP-NOW Receiver Starting...");
  
  Serial.println("Setting WiFi mode...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  // Small delay after WiFi setup
  
  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  Serial.println("Initializing ESP-NOW...");
  int result = esp_now_init();
  Serial.print("ESP-NOW init result: ");
  Serial.println(result);
  
  if (result != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
 
  Serial.println("Registering callback...");
  esp_now_register_recv_cb(onReceive);
 
  Serial.println("ESP-NOW Receiver initialized successfully");
  Serial.println("Waiting for messages...");
}

void loop() {
  Serial.println("Loop running...");  // Add this to see if loop is working
  delay(5000);  // Increase delay so it's not too chatty
}