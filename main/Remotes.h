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

    const int levelValues[8] =  {0, 20, 128, 256, 512, 1024, 2048, 4096};
    const int lightLevels = sizeof(this->levelValues) / sizeof(int) - 1;

    const int levelOn = 5;
   // const int levelOff = 0;

    
    const int pinLight1 = 0;
    const int pinLight2 = 1;
    const int pinLight3 = 2;

    int levelLight1;
    int levelLight2;
    int levelLight3;

    boolean arrayContain(unsigned long testArray[], unsigned long value);

    unsigned long lastTimeBtnClick = 0;
    unsigned long lastBtnClick;

    unsigned long getButton();
    unsigned long getButton(unsigned int attempts);


    byte getSavedLightLevel(int light);
    void saveLightLevel(int light, byte value);

    void setLight(int lightId, int value, boolean showMessage);

    void setAllLights(int value, boolean showMessage);
    void increaseAllLights(boolean showMessage);
    void decreaseAllLights(boolean showMessage);


  public:
    void setup(KeypadShield *keypadShield);
    void loop();
};

#endif
