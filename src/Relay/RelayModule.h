#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include "../Config/Config.h"

void initRelays();
void setRelayState(int index, int relayPin, bool state);
void toggleRelay(int index, int relayPin);

#endif
