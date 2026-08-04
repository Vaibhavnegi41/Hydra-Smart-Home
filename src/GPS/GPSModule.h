#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include "../Config/Config.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>

void initGPS();
void updateGPS();

#endif
