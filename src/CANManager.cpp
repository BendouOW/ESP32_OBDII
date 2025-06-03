#include "CANManager.h"

// Global variables
// bool autoIsOn = false;
// unsigned long lastAutoCheckTime = 0;
CAN_device_t CAN_cfg;
QueueHandle_t canRxQueue;

void setupCAN() {
  // Initialize CAN bus configuration
  CAN_cfg.speed = CAN_SPEED;
  CAN_cfg.tx_pin_id = CAN_TX_PIN;
  CAN_cfg.rx_pin_id = CAN_RX_PIN;
  CAN_cfg.rx_queue = xQueueCreate(10, sizeof(CAN_frame_t));
  canRxQueue = CAN_cfg.rx_queue;
  
  // Initialize CAN module
  ESP32Can.CANInit();
  Serial.println("CAN bus geïnitialiseerd");
}

void checkAutoPower() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastAutoCheckTime >= AUTO_DETECTION_INTERVAL) {
    lastAutoCheckTime = currentMillis;
    
    // Read auto detection pin
    bool autoSignal = digitalRead(AUTO_DETECTION_PIN);
    
    // Auto power management will be handled in main.cpp
    // to avoid circular dependencies with logging functions
  }
}

void processCANMessages() {
  CAN_frame_t frame;
  if (xQueueReceive(canRxQueue, &frame, 0) == pdTRUE) {
    processCAN(frame);
  }
}

void processCAN(CAN_frame_t &frame) {
  // Parse OBD2 PID responses from CAN frames
  if (frame.FIR.B.FF == CAN_frame_std) {
    uint32_t canId = frame.MsgID;
    
    // Standard OBD2 response format (Mode 41 for current data)
    if ((canId >= 0x7E8 && canId <= 0x7EF) || (canId >= 0x700 && canId <= 0x7FF)) {
      if (frame.data.u8[0] >= 2 && frame.data.u8[1] == 0x41) {
        uint8_t pid = frame.data.u8[2];
        
        // Process the PID according to OBD2 standards
        switch (pid) {
          case 0x04: // Engine load
            obd2Data.engineLoad = (frame.data.u8[3] * 100.0) / 255.0;
            break;
          case 0x05: // Coolant temp
            obd2Data.coolantTemp = frame.data.u8[3] - 40;
            break;
          case 0x0C: // Engine RPM
            obd2Data.engineRPM = ((frame.data.u8[3] * 256) + frame.data.u8[4]) / 4;
            break;
          case 0x0D: // Vehicle speed
            obd2Data.vehicleSpeed = frame.data.u8[3];
            break;
          case 0x11: // Throttle position
            obd2Data.throttlePosition = (frame.data.u8[3] * 100.0) / 255.0;
            break;
          case 0x2F: // Fuel Level
            obd2Data.fuelLevel = (frame.data.u8[3] * 100.0) / 255.0;
            break;
          case 0x0F: // Intake air temperature
            obd2Data.intakeTemp = frame.data.u8[3] - 40;
            break;
          case 0x5C: // Oil temperature
            obd2Data.oilTemp = frame.data.u8[3] - 40;
            break;
          case 0x70: // Turbo pressure
            obd2Data.turboPressure = ((frame.data.u8[3] * 256) + frame.data.u8[4]) / 1000.0;
            break;
        }
      }
    }
  }
}