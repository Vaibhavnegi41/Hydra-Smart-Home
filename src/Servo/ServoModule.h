#ifndef SERVO_MODULE_H
#define SERVO_MODULE_H

#include "../Config/Config.h"
#include <ESP32Servo.h>

void initServos();
void toggleDoor1();
void toggleDoor2();
void toggleCurtain();
void openMainDoor();
void closeMainDoor();

#endif
