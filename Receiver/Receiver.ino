#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266WebServer.h>

//Receiever


const char *ssid = "Miami-Dade"; // The name of the Wi-Fi network that will be created
const char *password = "Croqueta"; 
char last_msg[256] = "(none)";

uint8_t last_len = 0;
uint32_t last_msg_id = 0;  

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

  // Create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);


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

  size_t n = (len < sizeof(data) - 1) ? len : (sizeof(last_msg) - 1);
  memcpy(last_msg, data, n); 
  last_msg[n] = '\0';          // make sure it's null-terminated
  last_len = n;
  Serial.println();
 
  Serial.print("Length: ");
  Serial.println(len);
  
 
  esp_now_send(mac, (uint8_t*)&ack, sizeof(ack));

                 // Actually start the server
  Serial.println("HTTP server started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("---");

}

void setup() {
  Serial.begin(115200);
  delay(5000);  // Add this delay!
  Serial.println("ESP-NOW Receiver Starting...");
  
  WiFi.mode(WIFI_AP_STA);
  
  WiFi.softAP(ssid, password, 6); // 6 is the same channel for receiver 

  
  //start on 
  

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }
 
  Serial.println("Registering callback...");
  esp_now_register_recv_cb(onReceive);
 
  Serial.println("ESP-NOW Receiver initialized successfully");
  Serial.println("Waiting for messages...");
  
  Serial.println("Dual mode initialized");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);  // Register the root handler
  server.onNotFound(handleNotFound);  
  server.begin();             // Start the server
}

void loop() {

  server.handleClient(); 
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 10000) {  // Every 10 seconds
    Serial.println("Server running... waiting for connections");
    lastPrint = millis();
  }
  
  delay(10); // Increase delay so it's not too chatty
}

void handleRoot() {

  String html = "<!DOCTYPE html>";
  html += "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP8266 Server</title>";
  html += "</head><body>";
  html += "<h1>Hello from ESP8266!</h1>";
  html += "<p>Web server is working!</p>";
  html += "<p>";
  html += last_msg;
  html += "</p>";
  html += "<p>ESP-NOW receiver is active</p>";
  html += "<p>IP: 192.168.4.1</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
  Serial.println("Homepage served to client");  // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}