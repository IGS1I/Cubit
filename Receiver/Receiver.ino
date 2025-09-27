#include <ESP8266WiFi.h>
#include <espnow.h>
const char *ssid = "Miami-Dade"; // The name of the Wi-Fi network that will be created
const char *password = "Croqueta"; 



ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

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
  
   //WiFi.mode(WIFI_STA); Orginal one
  WiFi.mode(WIFI_AP_STA);
  //WiFi.mode(WIFI_AP);
  WiFi.softAP("YourSSID", "YourPassword", 6); 
  
  //on cinnection sender to reciver 
  //start on 
  WiFi.disconnect();
  

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
 
  Serial.println("Registering callback...");
  esp_now_register_recv_cb(onReceive);
 
  Serial.println("ESP-NOW Receiver initialized successfully");
  Serial.println("Waiting for messages...");
  // WiFi.softAP(ssid, password, 6);
  // Serial.print("IP address:\t");
  // Serial.println(WiFi.softAPIP()); 
  Serial.println("Dual mode initialized");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.println("Loop running...");  
  delay(4000);  // Increase delay so it's not too chatty
}