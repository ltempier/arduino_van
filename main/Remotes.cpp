
#include "Remotes.h"

void Remotes::setup() {
  pinMode(pin_interrupt, INPUT_PULLUP);
  mySwitch = RCSwitch();
  mySwitch.enableReceive(digitalPinToInterrupt(pin_interrupt));
}

unsigned long Remotes::checkClick() {
  if (mySwitch.available()) {
    unsigned long id = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    return id;
  }
  return 0;
}

void Remotes::loop(KeypadShield keypadShield) {

  if (mySwitch.available()) {
    unsigned long id = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    Serial.print("Decimal: ");
    Serial.println(id);

    keypadShield.writeTmpMessage("click");
  }
}
