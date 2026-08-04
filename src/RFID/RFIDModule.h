#ifndef RFID_MODULE_H
#define RFID_MODULE_H

#include "../Config/Config.h"
#include <SPI.h>
#include <MFRC522.h>

void initRFID();
void handleRFID();

#endif
