#ifndef OBD2_DATA_H
#define OBD2_DATA_H

// Structure for OBD2 sensor data
struct OBD2Data {
  float engineLoad;
  float coolantTemp;
  int engineRPM;
  int vehicleSpeed;
  float throttlePosition;
  float fuelLevel;
  float intakeTemp;
  float oilTemp;
  float turboPressure;
  float transmissionTemp;
  float exhaustTemp;
  float manifoldPressure;
};

// Global OBD2 data instance
extern OBD2Data obd2Data;

#endif // OBD2_DATA_H