/*
 * ESP-NOW RECEIVER with Web Interface

*/
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266WebServer.h>

// WiFi Access Point Configuration
const char *ssid = "Miami-Dade";     // WiFi network name
const char *password = "Croqueta";   // WiFi password

// Message storage
char last_msg[256] = "(none)";       // Stores the last received message
uint8_t last_len = 0;                // Length of last message
uint32_t last_msg_id = 0;            // Message ID (currently unused)

// Simple acknowledgment message to send back to sender
const char ack[] = "ACK";  

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

  // Create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);


/**
 * Callback function that runs whenever we receive an ESP-NOW message
 * This function:
 * 1. Prints sender's MAC address
 * 2. Prints the received message
 * 3. Stores the message for the web interface
 * 4. Sends an acknowledgment back to the sender
 */
void onReceive(uint8_t *mac, uint8_t *data, uint8_t len) {
  
  // Print sender's MAC address
  Serial.print("📨 Received from MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
 
  // Print the actual message
  Serial.print("Message: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();

  // Store message safely (fix the size comparison bug)
  size_t n = (len < sizeof(last_msg) - 1) ? len : (sizeof(last_msg) - 1);
  memcpy(last_msg, data, n); 
  last_msg[n] = '\0';          // Ensure null-terminated string
  last_len = n;
 
  Serial.print("Length: ");
  Serial.println(len);
  
  // Send acknowledgment back to sender
  esp_now_send(mac, (uint8_t*)ack, sizeof(ack) - 1); // -1 to exclude null terminator
  Serial.println(" ACK sent back to sender");
  Serial.println("---");

}

/**
 * Setup function - runs once when the ESP8266 starts
 * Initializes both ESP-NOW communication and web server
 */
void setup() {
  Serial.begin(115200);
  delay(5000);  // Give time for serial monitor to connect
  Serial.println(" ESP-NOW Receiver Starting...");
  
  // Set WiFi to both Access Point and Station mode
  WiFi.mode(WIFI_AP_STA);
  
  // Create WiFi access point on channel 6 (must match sender)
  WiFi.softAP(ssid, password, 6);
  Serial.println(" WiFi Access Point created");
  Serial.print("Connect to: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.print(" Web interface at: ");
  Serial.println(WiFi.softAPIP());

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println(" ESP-NOW initialization failed!");
    return;
  }

  Serial.println("Registering ESP-NOW callback...");
  esp_now_register_recv_cb(onReceive);
 
  Serial.println(" ESP-NOW Receiver initialized successfully");
  Serial.println(" Waiting for messages...");
  
  // Setup web server routes
  server.on("/", handleRoot);              // Main page
  server.onNotFound(handleNotFound);       // 404 handler
  server.begin();                          // Start the web server
  
  Serial.println("Web server started successfully");
  Serial.println("=== Setup Complete ===");
}

/**
 * Main loop - runs continuously
 * Handles web server requests and prints status updates
 */
void loop() {
  // Handle any incoming web requests
  server.handleClient(); 
  
  // Print status every 10 seconds so we know it's alive
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 10000) {
    Serial.println("🔄 Server running... waiting for connections");
    lastPrint = millis();
  }
  
  delay(10); // Small delay to prevent overwhelming the CPU
}

/**
 * Handles requests to the main webpage (192.168.4.1/)
 * Shows the last received ESP-NOW message
 */
void handleRoot() {
  // Build a nice-looking HTML page
  String html = "<!DOCTYPE html>";
  html += "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP-NOW Receiver</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 40px; background: #f0f0f0; }";
  html += ".container { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }";
  html += ".message { background: #e8f5e8; padding: 15px; border-radius: 5px; margin: 10px 0; border-left: 4px solid #4CAF50; }";
  html += ".status { color: #666; font-size: 0.9em; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1> ESP-NOW Receiver</h1>";
  html += "<div class='status'> Web server is working!</div>";
  html += "<h2>Last Received Message:</h2>";
  html += "<div class='message'>";
  html += last_msg;
  html += "</div>";
  html += "<div class='status'> ESP-NOW receiver is active</div>";
  html += "<div class='status'> IP: 192.168.4.1</div>";
  html += "<div class='status'> Page auto-refreshes every 5 seconds</div>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
  Serial.println(" Homepage served to client");
}

/**
 * Handles requests to non-existent pages
 * Returns a 404 error
 */
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
  Serial.println(" 404 error served to client");
}