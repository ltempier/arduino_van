#ifndef REMOTE_h
#define REMOTE_h

#include "Arduino.h"
#include <EEPROM.h>

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

    const byte lightLevels = 3;

    int levelLight1;
    int levelLight2;
    int levelLight3;

    unsigned long lastTimeBtnClick = 0;
    unsigned long lastBtnClick;

    unsigned long getButton();

    byte getSavedLightLevel(int light);
    void saveLightLevel(int light, byte value);


    void setLight1(int value, boolean showMessage);
    void setLight2(int value, boolean showMessage);
    void setLight3(int value, boolean showMessage);
    void setAllLights(int value, boolean showMessage);

  public:
    void setup(KeypadShield *keypadShield);
    void loop();
};

#endif
