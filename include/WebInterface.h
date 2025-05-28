#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "OBD2Data.h"
#include "FileManager.h"

// Web server and API functions
void setupWebServer();
void handleClientRequests();
void handleRoot();
void handleData();
void handleTestsList();
void handleDownload();
void handleDelete();
void handleDeleteAll();
void handleStartTest();
void handleStopTest();

void handleCaptivePortal();

// External variables
extern WebServer server;

#endif // WEB_INTERFACE_H