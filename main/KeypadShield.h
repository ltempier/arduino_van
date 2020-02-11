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

    Victron *victron;
    boolean backlightState;

    unsigned long lastTimeTurnDisplayOn = 0;
    unsigned long lastTimeTmpDisplay = 0;
    unsigned int tmpDisplayDuration = 0;

    enum buttons { RIGHT, UP, DOWN, LEFT, SELECT, NONE };
    unsigned long lastTimeBtnClick = 0;

    enum views { PPV, VPV, VB, ERR, HIST_PPV};
    int currentView = PPV;

    String currentTopLine;
    int displayedChargeStatePercent = -1;

    int getButton();

    void displayPvWatt(boolean force = false) ;
    void displayPvVolt(boolean force = false) ;
    void displayBVolt(boolean force = false) ;
    void displayErrCode(boolean force = false) ;
    void displayChargeState() ;

    void writeTopLine(String topLine, boolean force = false) ;

  public:
    void setup(Victron *victron);
    void loop() ;
    void turnDisplayOn();
    void turnDisplayOff();
    void writeTmpMessage(String msg, int tempDisplayTime = 3000);
};

#endif
