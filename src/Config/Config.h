#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi Settings
extern const char* ssid;
extern const char* password;

// SinricPro Settings
#define APP_KEY       "***************"
#define APP_SECRET    "***************"
#define SWITCH_ID_1   "690f18aefb7ad926788957"
#define SWITCH_ID_2   "690f198bfb7ad926788929"
#define SWITCH_ID_3   "690f19176cf03d7232cb18"

// Relay Settings
#define RELAY_ACTIVE_LOW true
#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif

#define RELAY_JAI_SHREE_RAM 32
#define RELAY_FAN           22
#define RELAY_LOWER_LIGHT   33
#define RELAY_UPPER_LIGHT   21

extern bool relayState[4];

// Servo Pins
#define DOOR1_SERVO 15
#define DOOR2_SERVO 2
#define CURTAIN_SERVO 4

extern bool doorOpen[2];
extern bool curtainOpen;
extern bool door1OpenByRFID;
extern unsigned long door1OpenUntil;
extern const unsigned long DOOR1_OPEN_MS;

// Motor Pins
#define LEFT_MOTOR_IN1 12
#define LEFT_MOTOR_IN2 13
#define RIGHT_MOTOR_IN1 14
#define RIGHT_MOTOR_IN2 27

// RFID Pins
#define SS_PIN  5
#define RST_PIN 25
extern byte allowedUID[4];

// GPS
extern double latitude;
extern double longitude;

// Web Server
extern const char* googleApiKey;

// Gas Sensor
extern const int mq2Pin;
extern const int buzzerPin;
extern const int gasThreshold;
extern const unsigned long MQ2_READ_INTERVAL;

extern float currentGasLevelPercent;
extern int currentSensorValue;
extern float currentVoltage;

extern bool buzzerActive;
extern bool modWarnActive;

// UI State
extern String lastDoorAccessMessage;
extern String lastAccessUID;
extern String lastAccessColor;

#endif
