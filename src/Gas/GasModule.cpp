#include "GasModule.h"

unsigned long lastMq2ReadMillis = 0;
unsigned long buzzerStartMillis = 0;
const unsigned long BUZZER_ON_DURATION = 300; 
const unsigned long BUZZER_OFF_DURATION = 200; 
unsigned long lastBuzzerToggle = 0;
bool buzzerOnState = false;

const unsigned long MOD_WARN_BUZZ_DURATION = 100;
const unsigned long MOD_WARN_PAUSE = 200;
unsigned long modWarnStart = 0;
unsigned long lastModWarnToggle = 0;
bool modWarnState = false;

void initGasSensor() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void updateBuzzerState() {
  unsigned long now = millis();

  if (buzzerActive) {
    if (now - lastBuzzerToggle >= (buzzerOnState ? BUZZER_ON_DURATION : BUZZER_OFF_DURATION)) {
      buzzerOnState = !buzzerOnState;
      lastBuzzerToggle = now;
      digitalWrite(buzzerPin, buzzerOnState ? HIGH : LOW);
    }
  } else if (modWarnActive) {
    if (now - lastModWarnToggle >= (modWarnState ? MOD_WARN_BUZZ_DURATION : MOD_WARN_PAUSE)) {
      modWarnState = !modWarnState;
      lastModWarnToggle = now;
      digitalWrite(buzzerPin, modWarnState ? HIGH : LOW);
    }
  } else {
    digitalWrite(buzzerPin, LOW);
    buzzerOnState = false;
    modWarnState = false;
  }
}

void updateGasSensor() {
  unsigned long now = millis();
  if (now - lastMq2ReadMillis >= MQ2_READ_INTERVAL) {
    lastMq2ReadMillis = now;

    int sensorValue = analogRead(mq2Pin);
    float voltage = sensorValue * (3.3 / 4095.0);
    float gasLevel = map(sensorValue, 0, 4095, 0, 100);

    currentSensorValue = sensorValue;
    currentVoltage = voltage;
    currentGasLevelPercent = gasLevel;

    if (sensorValue > gasThreshold) {
      buzzerActive = true;
      modWarnActive = false;
    } else if (sensorValue > 1450) {
      modWarnActive = true;
      buzzerActive = false;
    } else {
      buzzerActive = false;
      modWarnActive = false;
      digitalWrite(buzzerPin, LOW);
    }
  }
}
