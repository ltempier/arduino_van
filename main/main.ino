
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
}


void loop() {
  victron.loop();
  keypadShield.loop(victron);
  //remotes.loop(keypadShield);

  unsigned long id = remotes.checkClick();
  if (id > 0) {
    Serial.println(id);
    keypadShield.writeTmpMessage(String(id));
  }

}
