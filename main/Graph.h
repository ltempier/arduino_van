#ifndef GRAPH_h
#define GRAPH_h

#include <LiquidCrystal.h>

void KeypadShield::drawGraph(LiquidCrystal lcd, float data[40], int lcdPosition[2] = {0,1}) {

  int minValue = 0;
  int maxValue = data[0];
  for (byte i = 0; i < 40; i++) {
    if (data[i] > maxValue)
      maxValue = data[i];
    if (data[i] < minValue)
      minValue = data[i];
  }

  int mappedData[40];
  for (byte i = 0; i < 40; i++) {
    mappedData[i] = map(data[i], minValue, maxValue, 0, 8);
  }

  lcd.setCursor(lcdPosition[0], lcdPosition[1]);
  for (byte i = lcdPosition[0]; i < 16; i++)
    lcd.print(" ");

  for (byte iLcd = 0; iLcd < 8; iLcd++) {
    byte customChar[8];
    for (byte j = 0; j < 8; j++) {
      int bufferInt = 0;
      for (byte i = 0 ; i < 5; i++) {
        int pixelValue = mappedData[iLcd * 5 + i];
        if (pixelValue >= 8 - j)
          bitWrite(bufferInt, 4 - i, 1);
        else
          bitWrite(bufferInt, 4 - i, 0);
      }
      customChar[j] = (byte) bufferInt;
    }
    lcd.createChar((byte)iLcd, customChar);
    lcd.setCursor(lcdPosition[0] + iLcd, lcdPosition[1]);
    lcd.write((byte)iLcd);
  }
}

#endif
