#include "RFIDModule.h"
#include "../Servo/ServoModule.h"
#include "../Relay/RelayModule.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);

void initRFID() {
  SPI.begin(18, 19, 23, SS_PIN);
  mfrc522.PCD_Init();
}

void handleRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String scannedUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      scannedUID += String(mfrc522.uid.uidByte[i], HEX);
    }

    bool auth = true;
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
        auth = false;
        break;
      }
    }

    if (auth) {
      lastDoorAccessMessage = "Authorized Card Access: Main Door Open & Lower Floor Light ON";
      lastAccessUID = scannedUID;
      lastAccessColor = "#28A745"; 
      
      openMainDoor();
      setRelayState(2, RELAY_LOWER_LIGHT, true);
      door1OpenByRFID = true;
      door1OpenUntil = millis() + DOOR1_OPEN_MS;
    } else {
      lastDoorAccessMessage = " Unauthorized Card Access: Denied!";
      lastAccessUID = scannedUID;
      lastAccessColor = "#DC3545"; 
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  if (door1OpenByRFID && millis() > door1OpenUntil) {
    closeMainDoor();
    door1OpenByRFID = false;
    setRelayState(2, RELAY_LOWER_LIGHT, false);
    
    lastDoorAccessMessage = "🔒 Auto-Close: Door 1 Closed & Light OFF";
    lastAccessUID = "System Timer";
    lastAccessColor = "#6C757D"; 
  }
}
