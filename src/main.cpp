#include <Arduino.h>
#include <time.h>
#include "config.h"
#include "OBD2Data.h"
#include "WiFiManager.h"
#include "CANManager.h"
#include "FileManager.h"
#include "WebInterface.h"

// External variable declarations for main.cpp
extern bool autoIsOn;
extern unsigned long lastAutoCheckTime;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("OBD2 Scanner gestart...");
  
  // Initialize GPIO pins
  pinMode(CONNECTION_LED_PIN, OUTPUT);
  digitalWrite(CONNECTION_LED_PIN, LOW);
  pinMode(AUTO_DETECTION_PIN, INPUT);
  
  // Initialize SPIFFS file system
  if (!setupSPIFFS()) {
    Serial.println("SPIFFS initialisatie mislukt!");
    return;
  }
  
  // Initialize WiFi Access Point
  setupWiFi();
  
  // Initialize CAN bus
  setupCAN();

  // Initialize time with timezone (example: GMT+1 for Europe/Amsterdam)
  // Adjust the first parameter for your timezone: 
  // GMT+1 = 3600, GMT+2 = 7200, GMT-5 = -18000, etc.
  configTime(3600, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time synchronization
  Serial.print("Waiting for time sync");
  for (int i = 0; i < 20; i++) {
    time_t now = time(nullptr);
    if (now > 1000000000) { // Check if time is reasonable (after year 2001)
      break;
    }
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Print current time for verification
  time_t now = time(nullptr);
  Serial.printf("Current time: %s", ctime(&now));
  
  // Initialize file system and create sample files
  initLogDirectory();
  
  // Initialize web server
  setupWebServer();
  
  Serial.println("Systeem volledig geïnitialiseerd en klaar voor gebruik!");
}

void loop() {
  // Process DNS requests for captive portal
  processDNS();
  
  // Handle incoming HTTP requests
  handleClientRequests();
  
  // Check WiFi connections
  checkWiFiConnections();
  
  // Check auto power status with automatic logging
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastAutoCheckTime >= AUTO_DETECTION_INTERVAL) {
    lastAutoCheckTime = currentMillis;
    
    bool autoSignal = digitalRead(AUTO_DETECTION_PIN);
    
    if (autoSignal && !autoIsOn) {
      autoIsOn = true;
      Serial.println("Car started. Starting automatic logging...");
      startLogging();
    } 
    else if (!autoSignal && autoIsOn) {
      autoIsOn = false;
      Serial.println("Car stopped. Stopping automatic logging...");
      stopLogging();
    }
  }
  
  // Process CAN messages
  processCANMessages();
  
  // Handle data logging
  processLogging();
}