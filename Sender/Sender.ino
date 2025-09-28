/*
 * ESP-NOW SENDER
 * 
 * This device sends "All is Good" messages every 5 seconds
 * to a specific receiver device using ESP-NOW protocol.
 * 
 * ESP-NOW is a wireless communication protocol that allows
 * ESP devices to communicate without connecting to WiFi.
 */

#include <ESP8266WiFi.h>
#include <espnow.h>

// IMPORTANT: Replace with your receiver's actual MAC address
// You can find the receiver's MAC by checking its serial output
uint8_t receiverMAC[] = {0x48, 0x55, 0x19, 0xed, 0x33, 0x11};

// Status tracking
bool messageAcknowledged = false;  // Tracks if receiver responded
unsigned long lastSendTime = 0;   // Tracks when we last sent a message

/**
 * Callback function that runs after we try to send a message
 * Tells us if the message was delivered successfully
 */
void onSent(uint8_t *mac, uint8_t status) {
  Serial.print(" Send status to ");
  
  // Print the MAC address we sent to
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  
  // Print success or failure
  Serial.print(" - ");
  if (status == 0) {
    Serial.println(" Success");
  } else {
    Serial.println(" Failed");
  }
}

/**
 * Callback function that runs when we receive a message back
 * This would handle acknowledgments from the receiver
 * Currently not used, but kept for future functionality
 */
void onReceive(uint8_t *mac, uint8_t *data, uint8_t len) {

  // Check if the response is from our intended receiver
  if (memcmp(mac, receiverMAC, 6) == 0) {
    Serial.print(" Received ACK from receiver: ");
    for (int i = 0; i < len; i++) {
      Serial.print((char)data[i]);
    }
    Serial.println();
    messageAcknowledged = true;
  }

}

/**
 * Setup function - runs once when the ESP8266 starts
 * Initializes ESP-NOW communication
 */
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(" ESP-NOW Sender Starting...");

  // Set WiFi to station mode (not creating an access point)
  WiFi.mode(WIFI_STA);
  Serial.println(" WiFi set to Station mode");

  // Set channel to 6 (must match receiver's channel)
  wifi_set_channel(6);
  WiFi.disconnect();  // Make sure we're not connected to any WiFi
  Serial.println(" Channel set to 6");

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println(" ESP-NOW initialization failed!");
    return;
  }

  // Set up ESP-NOW roles and add the receiver as a peer
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);  // Seting up sender as main.
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);  // Add receiver as slave
  

  Serial.print(" Target receiver MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", receiverMAC[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // Register callback functions
  esp_now_register_send_cb(onSent);      // Called when message is sent
  esp_now_register_recv_cb(onReceive);   // Called when we receive a response
  
  Serial.println(" ESP-NOW Sender initialized successfully");
  Serial.println(" Ready to send messages...");
  Serial.println("=== Setup Complete ===");
} 

/**
 * Main loop - runs continuously
 * Sends "All is Good" message every 5 seconds
 */
void loop() {
  // Only send if enough time has passed
  
  if (millis() - lastSendTime >= 5000) {
    const char msg[] = "All is Good";
    
    Serial.print(" Sending: ");
    Serial.println(msg);
    
    // Reset acknowledgment flag
    messageAcknowledged = false;
    
    // Send the message
    uint8_t result = esp_now_send(receiverMAC,(uint8_t*)msg, sizeof(msg) - 1);
    
    if (result != 0) {
      Serial.println(" Send failed immediately!");
    }
    
    lastSendTime = millis();  // Update last send time
  }
  
  delay(100);  // Small delay to prevent overwhelming the CPU
}