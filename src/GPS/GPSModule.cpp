#include "GPSModule.h"

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

void initGPS() {
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void updateGPS() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }
  }
}
