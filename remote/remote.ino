/*
 * dispay remote ID
 */

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


void setup() {
  Serial.begin(19200);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    Serial.print("Decimal: ");
    Serial.println(mySwitch.getReceivedValue());    
    mySwitch.resetAvailable();
  }
}
