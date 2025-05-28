#ifndef OBD2_DATA_H
#define OBD2_DATA_H

// Structure for OBD2 sensor data
struct OBD2Data {
  float engineLoad = 45.0;
  float coolantTemp = 92.5;
  int engineRPM = 1500;
  int vehicleSpeed = 65;
  float throttlePosition = 25.8;
  float fuelLevel = 75.2;
  float intakeTemp = 40.6;
  float oilTemp = 98.7;
  float turboPressure = 1.2;
  float transmissionTemp = 85.3;
  float exhaustTemp = 215.8;
  float manifoldPressure = 102.4;
};

// Global OBD2 data instance
extern OBD2Data obd2Data;

#endif // OBD2_DATA_H