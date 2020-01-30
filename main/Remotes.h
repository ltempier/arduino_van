#ifndef REMOTE_h
#define REMOTE_h

#include "Arduino.h"

#include <RCSwitch.h>
#include "RemotesIds.h"
#include "KeypadShield.h"

class Remotes {
  private:

    const int pin_interrupt = 2;
    RCSwitch mySwitch;
    boolean arrayContain(unsigned long testArray[], unsigned long value);

    int levelLight1 = 0;
    int levelLight2 = 0;
    int levelLight3 = 0;

    int lightLevels = 5;
    unsigned long lastTimeBtnClick = 0;
    unsigned long lastBtnClick;

  public:
    Remotes();
    void setLight1(int value);
    void setLight2(int value);
    void setLight3(int value);

    void setup();
    void loop(KeypadShield &keypadShield);
    unsigned long getButton();
};

#endif
