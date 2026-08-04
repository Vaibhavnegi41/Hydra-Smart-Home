#include "ServoModule.h"

Servo door1;
Servo door2;
Servo curtain;

void initServos() {
  door1.attach(DOOR1_SERVO);
  door2.attach(DOOR2_SERVO);
  curtain.attach(CURTAIN_SERVO);
  door1.write(0); 
  door2.write(0); 
  curtain.write(10);
}

void toggleDoor1() {
  doorOpen[0] = !doorOpen[0];
  door1.write(doorOpen[0] ? 90 : 0);
}

void toggleDoor2() {
  doorOpen[1] = !doorOpen[1];
  door2.write(doorOpen[1] ? 90 : 0);
}

void toggleCurtain() {
  curtainOpen = !curtainOpen;
  curtain.write(curtainOpen ? 90 : 10);
}

void openMainDoor() {
  doorOpen[0] = true;
  door1.write(90);
}

void closeMainDoor() {
  doorOpen[0] = false;
  door1.write(0);
}
