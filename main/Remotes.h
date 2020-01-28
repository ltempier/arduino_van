#ifndef REMOTE_h
#define REMOTE_h

#include <Arduino.h>
#include <Wire.h>

#include <RCSwitch.h>
#include "RemotesIds.h"
#include "KeypadShield.h"


class Remotes {
  private:
    const int pin_interrupt = 11; 
    RCSwitch mySwitch;
    
  public:
    void setup();
    void loop(KeypadShield keypadShield);
};

#endif
