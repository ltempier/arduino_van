#ifndef VICTRON_h
#define VICTRON_h

#include <Arduino.h>

/*
    ERR CODE:
    0 No error
    2 Battery voltage too high
    17 Charger temperature too high
    18 Charger over current
    19 Charger current reversed
    20 Bulk time limit exceeded
    21 Current sensor issue (sensor bias/sensor broken)
    26 Terminals overheated
    33 Input voltage too high (solar panel)
    34 Input current too high (solar panel)
    38 Input shutdown (due to excessive battery voltage)
    116 Factory calibration data lost
    117 Invalid/incompatible firmware
    119 User settings invalid
*/

class Victron {
  private:
    float pvWatt = 0;
    float pvVolt = 0;
    float bVolt = 0;
    float histPvWatt[40] = {2, 3, 4, 5, 6, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 5, 2, 7, 2, 2, 4, 6, 3, 5, 8, 4, 5, 2, 7, 2, 4, 6, 3, 5, 8, 4, 5, 2, 7, 2};

    int chargeState = 0;
    int errCode = 0;

    const int comDelayMicros = 800;
    void fetch();

  public:
    void setup();
    void loop();

    float getPvWatt();
    void fillHistPvWatt(float bufferHistPvWatt[]);

    float getBVolt();
    float getPvVolt();
    int getChargeStatePercent();
    int getErrCode();
};



#endif
