#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include "config.h"

// WiFi and network management functions
void setupWiFi();
void checkWiFiConnections();
void processDNS();

// External variables
extern DNSServer dnsServer;
extern bool clientConnected;
extern unsigned long lastClientCheck;

#endif // WIFI_MANAGER_H