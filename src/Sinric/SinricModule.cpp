#include "SinricModule.h"
#include "../Relay/RelayModule.h"

bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == SWITCH_ID_1) {
    setRelayState(0, RELAY_JAI_SHREE_RAM, state);
  } else if (deviceId == SWITCH_ID_2) {
    setRelayState(1, RELAY_FAN, state);
  } else if (deviceId == SWITCH_ID_3) {
    setRelayState(3, RELAY_UPPER_LIGHT, state);
  }
  return true;
}

void initSinricPro() {
  SinricProSwitch &sw1 = SinricPro[SWITCH_ID_1];
  SinricProSwitch &sw2 = SinricPro[SWITCH_ID_2];
  SinricProSwitch &sw3 = SinricPro[SWITCH_ID_3];
  
  sw1.onPowerState(onPowerState);
  sw2.onPowerState(onPowerState);
  sw3.onPowerState(onPowerState);
  
  SinricPro.onConnected([](){ Serial.println("Connected to Sinric Pro!"); });
  SinricPro.onDisconnected([](){ Serial.println("Disconnected from Sinric Pro!"); });
  
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void handleSinricPro() {
  SinricPro.handle();
}
