#include "WiFiManager.h"

// Global variables
DNSServer dnsServer;
bool clientConnected = false;
unsigned long lastClientCheck = 0;

// Network configuration
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void setupWiFi() {
  // Configure WiFi as Access Point
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.println("Access Point gestart:");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP-adres: ");
  Serial.println(WiFi.softAPIP());
  
  // Initialize DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", local_ip);
}

void checkWiFiConnections() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastClientCheck >= CLIENT_CHECK_INTERVAL) {
    lastClientCheck = currentMillis;
    
    if (WiFi.softAPgetStationNum() > 0) {
      if (!clientConnected) {
        clientConnected = true;
        Serial.println("Client verbonden met Access Point!");
        digitalWrite(CONNECTION_LED_PIN, HIGH);
      }
    } else {
      if (clientConnected) {
        clientConnected = false;
        Serial.println("Alle clients verbroken. Wachten op nieuwe verbindingen...");
        digitalWrite(CONNECTION_LED_PIN, LOW);
      }
    }

    Serial.print("Aantal verbonden clients: ");
    Serial.println(WiFi.softAPgetStationNum());
  }
}

void processDNS() {
  dnsServer.processNextRequest();
}