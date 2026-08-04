#include "FaceRecogModule.h"
#include "../Servo/ServoModule.h"

void handleFaceRecognitionSerial() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "OPEN") {
      lastDoorAccessMessage = "Face Recognized: Opening Main Door";
      lastAccessUID = "Face Recognition";
      lastAccessColor = "#17A2B8"; 
      
      openMainDoor();
    } else if (command == "CLOSE") {
      lastDoorAccessMessage = "Face Session Ended: Closing Main Door";
      lastAccessUID = "Face Recognition";
      lastAccessColor = "#6C757D"; 
    
      closeMainDoor();
    }
  }
}
