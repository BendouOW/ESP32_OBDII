#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include "config.h"
#include "OBD2Data.h"

// CAN and OBD2 management functions
void setupCAN();
void processCANMessages();
void processCAN(CAN_frame_t &frame);
void checkAutoPower();

// External variables
extern bool autoIsOn;
extern unsigned long lastAutoCheckTime;
extern QueueHandle_t canRxQueue;

#endif // CAN_MANAGER_H