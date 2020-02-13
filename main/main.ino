
#include "Victron.h"
#include "KeypadShield.h"
#include "Remotes.h"

Victron victron;
KeypadShield keypadShield;
Remotes remotes;

void setup() {
  Serial.begin(19200);
  victron.setup();
  keypadShield.setup(&victron);
  remotes.setup(&keypadShield);
  keypadShield.writeTmpMessage("setup done", 1000);
}

void loop() {
  victron.loop();
  keypadShield.loop();
  remotes.loop();
}
