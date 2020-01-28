#ifndef REMOTE_h
#define REMOTE_h

//#include <Arduino.h>
//#include <Wire.h>

#include <RCSwitch.h>
#include "RemotesIds.h"
#include "KeypadShield.h"


class Remotes {
  private:
    const int pin_interrupt = 2; 
    RCSwitch mySwitch;
    
  public:
    void setup();
    void loop(KeypadShield keypadShield);
    unsigned long checkClick();
};

#endif
