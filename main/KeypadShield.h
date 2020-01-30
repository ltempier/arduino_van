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

    unsigned long lastTimeTurnDisplayOn = 0;
    unsigned long lastTimeTmpDisplay = 0;
    unsigned int tmpDisplayDuration = 0;
    boolean backlightState;

    enum buttons { RIGHT, UP, DOWN, LEFT, SELECT, NONE };
    unsigned long lastTimeBtnClick = 0;

    enum views { PPV, VPV, VB, ERR};
    int currentView = PPV;

    String currentTopLine;
    int displayedChargeStatePercent = -1;

    void setBacklightPercent(byte percent);
    int getButton();

    void displayPvWatt(Victron victron, boolean force = false) ;
    void displayPvVolt(Victron victron, boolean force = false) ;
    void displayBVolt(Victron victron, boolean force = false) ;
    void displayErrCode(Victron victron, boolean force = false) ;
    void displayChargeState(Victron victron) ;

    void writeTopLine(String topLine, boolean force = false) ;

  public:
    void turnDisplayOn();
    void turnDisplayOff();
    void setup();
    void loop(Victron &victron) ;
    void writeTmpMessage(String msg, int tempDisplayTime = 3000);
};

#endif
