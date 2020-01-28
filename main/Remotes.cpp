
#include "Remotes.h"

void Remotes::setup() {
  mySwitch = RCSwitch();
  mySwitch.enableReceive(digitalPinToInterrupt(pin_interrupt));
}

void Remotes::loop(KeypadShield keypadShield) {

  if (mySwitch.available()) {
    unsigned long id = mySwitch.getReceivedValue();

    Serial.print("Decimal: ");
    Serial.println(id);

    keypadShield.writeTmpMessage(String(id, DEC));
    mySwitch.resetAvailable();
  }
}
