
#include "Victron.h"
#include "KeypadShield.h"
#include "Remotes.h"

Victron victron;
KeypadShield keypadShield;
Remotes remotes;

void setup() {
  victron.setup(); //Serial 19200
  keypadShield.setup();
  remotes.setup();
  keypadShield.writeTmpMessage("setup done", 1000);
}


void loop() {
  victron.loop();
  keypadShield.loop(victron);
  remotes.loop(keypadShield);
}
