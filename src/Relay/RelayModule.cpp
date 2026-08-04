#include "RelayModule.h"

void initRelays() {
  pinMode(RELAY_JAI_SHREE_RAM, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_LOWER_LIGHT, OUTPUT);
  pinMode(RELAY_UPPER_LIGHT, OUTPUT);

  digitalWrite(RELAY_JAI_SHREE_RAM, RELAY_OFF);
  digitalWrite(RELAY_FAN, RELAY_OFF);
  digitalWrite(RELAY_LOWER_LIGHT, RELAY_OFF);
  digitalWrite(RELAY_UPPER_LIGHT, RELAY_OFF);

  relayState[0] = false;
  relayState[1] = false;
  relayState[2] = false;
  relayState[3] = false;
}

void setRelayState(int index, int relayPin, bool state) {
  relayState[index] = state;
  digitalWrite(relayPin, state ? RELAY_ON : RELAY_OFF);
}

void toggleRelay(int index, int relayPin) {
  setRelayState(index, relayPin, !relayState[index]);
}
