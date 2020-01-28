
#include "KeypadShield.h"
#include "Remotes.h"
#include "Victron.h"

Remotes remotes;
KeypadShield keypadShield;
Victron victron;

void setup() {
  victron.setup(); //Serial 19200
  keypadShield.setup();
  //remotes.setup();
}


void loop() {
  victron.loop();
  keypadShield.loop(victron);
  //remotes.loop(keypadShield);
}
