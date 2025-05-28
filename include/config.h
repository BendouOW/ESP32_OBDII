#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define CONNECTION_LED_PIN 2
#define AUTO_DETECTION_PIN 34

// Timing intervals
#define AUTO_DETECTION_INTERVAL 1000
#define CLIENT_CHECK_INTERVAL 5000

// Network settings
#define WIFI_SSID "OBD2-ESP32"
#define WIFI_PASSWORD "12345678"
#define DNS_PORT 53
#define SERVER_PORT 80

// File system settings
#define LOG_DIR "/logs"
#define LOG_INTERVAL 100

// CAN bus settings
#define CAN_TX_PIN GPIO_NUM_5
#define CAN_RX_PIN GPIO_NUM_4
#define CAN_SPEED CAN_SPEED_500KBPS

#endif // CONFIG_H