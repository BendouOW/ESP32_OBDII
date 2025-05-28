#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <time.h>
#include "config.h"
#include "OBD2Data.h"

// File and logging management functions
bool setupSPIFFS();
void initLogDirectory();
void createNewLogFile();
void logOBD2Data();
void processLogging();
void startLogging();
void stopLogging();
String humanReadableSize(size_t bytes);
String extractDateTime(String filename);

// External variables
extern String currentLogFile;
extern bool isLogging;
extern unsigned long lastLogTime;
extern int testCounter;
extern bool testMode;
#endif // FILE_MANAGER_H