#ifndef REMOTE_h
#define REMOTE_h

#include "Arduino.h"

#include <RCSwitch.h>
#include <Adafruit_PWMServoDriver.h>

#include "RemotesIds.h"
#include "KeypadShield.h"

class Remotes {
  private:

    const int pin_interrupt = 2;
    RCSwitch mySwitch;
    Adafruit_PWMServoDriver pwm;

    KeypadShield *keypadShield;

    boolean arrayContain(unsigned long testArray[], unsigned long value);

    int levelLight1 = 0;
    int levelLight2 = 0;
    int levelLight3 = 0;

    int lightLevels = 5;
    unsigned long lastTimeBtnClick = 0;
    unsigned long lastBtnClick;

    unsigned long getButton();
    void setLight1(int value, boolean showMessage = true);
    void setLight2(int value, boolean showMessage = true);
    void setLight3(int value, boolean showMessage = true);

  public:
    void setup(KeypadShield *keypadShield);
    void loop();
};

#endif
