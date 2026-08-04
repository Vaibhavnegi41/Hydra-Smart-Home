/*************************************************************
 🏠 Smart Hydra Home + RFID + GPS + Alexa (Sinric Pro) + MQ2 Gas Alert + Face Recognition Door
 *************************************************************/

#include "src/Config/Config.h"
#include "src/Relay/RelayModule.h"
#include "src/Servo/ServoModule.h"
#include "src/Motor/MotorModule.h"
#include "src/RFID/RFIDModule.h"
#include "src/GPS/GPSModule.h"
#include "src/Gas/GasModule.h"
#include "src/Web/WebModule.h"
#include "src/Sinric/SinricModule.h"
#include "src/FaceRecog/FaceRecogModule.h"
#include <WiFi.h>

const unsigned long WIFI_RECONNECT_INTERVAL = 5000;
unsigned long lastWiFiReconnectAttempt = 0;

const unsigned long IP_PRINT_INTERVAL = 10000; 
unsigned long lastIPPrintMillis = 0;

void printIPContinuously() {
  unsigned long now = millis();
  if (WiFi.status() == WL_CONNECTED && (now - lastIPPrintMillis >= IP_PRINT_INTERVAL)) {
    lastIPPrintMillis = now;
    Serial.print("🌐 Current IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nSmart Hydra Home Booting...");

  initRelays();
  initServos();
  initMotors();
  initRFID();
  Serial.println("RFID Ready...");

  initGPS();
  Serial.println("📡 GPS Initialized...");

  initGasSensor();
  Serial.println("MQ2 Gas Sensor Ready...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.print("Connecting to WiFi (non-blocking) ...");

  initSinricPro();
  initWebServer();
  Serial.println("Web Server Running...");
}

void loop() {
  handleClient();
  handleSinricPro();

  unsigned long now = millis();
  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWiFiReconnectAttempt >= WIFI_RECONNECT_INTERVAL) {
      lastWiFiReconnectAttempt = now;
      Serial.println("WiFi disconnected! Attempting reconnect...");
      WiFi.reconnect();
    }
  }

  printIPContinuously(); 

  handleRFID();
  updateGPS();
  updateGasSensor();
  updateBuzzerState();
  handleFaceRecognitionSerial();

  yield();
}
