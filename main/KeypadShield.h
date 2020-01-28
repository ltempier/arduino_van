#ifndef KEYPADSHIED_h
#define KEYPADSHIED_h

#include "Arduino.h"
#include "Victron.h"
#include "ProgressBarChars.h"
#include <LiquidCrystal.h>

class KeypadShield {
  private:
    const byte pin_button = A0;
    const byte pin_LCD_BL = 10;
    LiquidCrystal lcd =  LiquidCrystal (8, 9, 4, 5, 6, 7);

    unsigned long lastBtnClick;
    unsigned long lastTempDisplay = 0;
    int tempDisplayTime = 5000;

    enum buttons { RIGHT, UP, DOWN, LEFT, SELECT, NONE };
    enum views { PPV, VPV, VB, ERR};
    int currentView = PPV;
    boolean backlightState;

    String currentTopLine;
    int displayedChargeStatePercent = -1;

    void setBacklightPercent(byte percent);
    int getButton();

    void displayPvWatt(Victron victron);
    void displayPvVolt(Victron victron);
    void displayBVolt(Victron victron) ;
    void displayErrCode(Victron victron);
    void displayChargeState(Victron victron) ;

    void writeTopLine(String topLine) ;

  public:
    void turnDisplayOn();
    void turnDisplayOff();
    void setup();
    void loop(Victron victron) ;
    void writeTmpMessage(String msg);
    void writeTmpMessage(String msg, int tempDisplayTime);
};

#endif
