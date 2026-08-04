#include "Config.h"

// WiFi
const char* ssid = "HiFi";
const char* password = "*******";

// Relay State
bool relayState[4] = { false, false, false, false };

// Servo State
bool doorOpen[2] = {false, false};
bool curtainOpen = false;
bool door1OpenByRFID = false;
unsigned long door1OpenUntil = 0;
const unsigned long DOOR1_OPEN_MS = 5000;

// RFID
byte allowedUID[4] = {0xC7, 0x31, 0x7B, 0x05};

// GPS
double latitude = 0.0;
double longitude = 0.0;

// Web Server
const char* googleApiKey = "********************";

// Gas Sensor
const int mq2Pin = 34;
const int buzzerPin = 26;
const int gasThreshold = 1500;
const unsigned long MQ2_READ_INTERVAL = 1000;

float currentGasLevelPercent = 0.0;
int currentSensorValue = 0;
float currentVoltage = 0.0;

bool buzzerActive = false;
bool modWarnActive = false;

// UI State
String lastDoorAccessMessage = "No recent access attempt.";
String lastAccessUID = "N/A";
String lastAccessColor = "#928DAB";
