
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

unsigned long bufferTime = 100000;

void loop() {
  keypadShield.loop();
  remotes.loop();
  victron.loop();

  /*
  unsigned long now = millis();
  if(now - bufferTime > 5000 || now < bufferTime){ // every 5sec
    bufferTime = now;
    victron.loop();
  }
  */
}
