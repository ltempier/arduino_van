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

    boolean backlightState;

    unsigned long lastTimeTurnDisplayOn = 0;
    unsigned long lastTimeTmpDisplay = 0;
    unsigned int tmpDisplayDuration = 0;

    enum buttons { RIGHT, UP, DOWN, LEFT, SELECT, NONE };
    unsigned long lastTimeBtnClick = 0;

    enum views { PPV, VPV, VB, ERR, HIST_PPV};
    int currentView = PPV;

    int * topLineChars[16] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    int * bottomLineChars[16] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};

    //TODO remove
    String currentTopLine;
    int displayedChargeStatePercent = -1;
    //END TODO

    int getButton();

    void displayPvWatt(Victron victron, boolean force = false) ;
    void displayPvVolt(Victron victron, boolean force = false) ;
    void displayBVolt(Victron victron, boolean force = false) ;
    void displayErrCode(Victron victron, boolean force = false) ;
    //void displayHistPvWattCode(Victron victron, boolean force = false) ;
    void displayChargeState(Victron victron) ;

    //void writeTopLineChars(char chars[], boolean force = false);

    void writeTopLine(String topLine, boolean force = false) ;


  public:
    void setup();
    void loop(Victron &victron) ;
    void turnDisplayOn();
    void turnDisplayOff();
    void writeTmpMessage(String msg, int tempDisplayTime = 3000);
};

#endif
