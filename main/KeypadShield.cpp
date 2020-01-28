
#include "KeypadShield.h"

void KeypadShield::turnDisplayOn() {
  digitalWrite(pin_LCD_BL, HIGH);
  this->backlightState = true;
}

void KeypadShield::turnDisplayOff() {
  digitalWrite(pin_LCD_BL, LOW);
  this->backlightState = false;
}

int KeypadShield::getButton() {
  static const int values[] = { 50, 150, 300, 500, 750 };
  int value = analogRead(pin_button);
  if (value > values[4] || millis() - lastBtnClick < 300)
    return NONE;

  lastBtnClick = millis();
  if (!backlightState) {
    this->turnDisplayOn();
    return NONE;
  }

  for (int i = 0; i < 5; i++) {
    if (value < values[i])
      return i;
  }
}


void KeypadShield::writeTopLine(String topLine) {
  
  if (this->currentTopLine.equals(topLine))
    return;

  if (this->lastTempDisplay > 0 && millis() - this->lastTempDisplay  < this->tempDisplayTime)
    return;

  this->lcd.setCursor(0, 0);
  this->lcd.print(topLine);

  this->lastTempDisplay = 0;
  this->currentTopLine = topLine;
}


void KeypadShield::writeTmpMessage(String msg) {
  this->writeTmpMessage(msg, 5000);
}

void KeypadShield::writeTmpMessage(String msg, int tempDisplayTime) {
  while (msg.length() < 16)
    msg += " ";

  this->writeTopLine(msg);
  this->lastTempDisplay = millis();
  this->tempDisplayTime = tempDisplayTime;
}


void KeypadShield::displayPvWatt(Victron victron) {
  String topLine = "Ppv:";
  String sPvWatt = String(victron.getPvWatt(), 2);
  sPvWatt += " W";
  for (int i = topLine.length() ; i < 16 - sPvWatt.length(); i++)
    topLine += " ";
  topLine += sPvWatt;
  this->writeTopLine(topLine);
}

void KeypadShield::displayPvVolt(Victron victron) {
  String topLine = "Vpv:";
  String sPvVolt = String(victron.getPvVolt(), 2);
  sPvVolt += " V";
  for (int i = topLine.length() ; i < 16 - sPvVolt.length(); i++)
    topLine += " ";
  topLine += sPvVolt;
  this->writeTopLine(topLine);
}

void KeypadShield::displayBVolt(Victron victron) {
  String topLine = "Vb:";
  String sBVolt = String(victron.getBVolt(), 2);
  sBVolt += " V";
  for (int i = topLine.length() ; i < 16 - sBVolt.length(); i++)
    topLine += " ";
  topLine += sBVolt;
  this->writeTopLine(topLine);
}

void KeypadShield::displayErrCode(Victron victron) {
  String topLine = "Err:";
  String sErrCode = String(victron.getErrCode());
  for (int i = topLine.length() ; i < 16 - sErrCode.length(); i++)
    topLine += " ";
  topLine += sErrCode;
  this->writeTopLine(topLine);
}

void KeypadShield::displayChargeState(Victron victron) {
  const int percent = victron.getChargeStatePercent();
  if (this->displayedChargeStatePercent == percent)
    return;

  const byte progressBarSize = 12;
  byte nbVerticalLines = map(percent, 0, 100, 0, progressBarSize * 2 - 2);
  this->lcd.setCursor(0, 1);
  for (byte i = 0; i < progressBarSize; ++i) {
    if (i == 0) { // Premiére case
      if (nbVerticalLines > 0) {
        this->lcd.write((byte) 1); // Char début 1 / 1
        nbVerticalLines -= 1;
      } else
        this->lcd.write((byte) 0); // Char début 0 / 1
    } else if (i == progressBarSize - 1) { // Derniére case
      if (nbVerticalLines > 0)
        this->lcd.write((byte) 6); // Char fin 1 / 1
      else
        this->lcd.write((byte) 5); // Char fin 0 / 1
    } else { // Autres cases
      if (nbVerticalLines >= 2) {
        this->lcd.write((byte) 4); // Char div 2 / 2
        nbVerticalLines -= 2;
      } else if (nbVerticalLines == 1) {
        this->lcd.write((byte) 3); // Char div 1 / 2
        nbVerticalLines -= 1;
      } else
        this->lcd.write((byte) 2); // Char div 0 / 2
    }
  }

  String sPercent = String(percent) + "%";
  while (sPercent.length() < 16 - progressBarSize)
    sPercent = " " + sPercent;
  this->lcd.print(sPercent);
  this->displayedChargeStatePercent = percent;
}


void KeypadShield::setup() {

  pinMode(pin_LCD_BL, OUTPUT);
  this->turnDisplayOn();

  this->lcd.begin(16, 2);
  this->lcd.createChar(0, START_DIV_0_OF_1);
  this->lcd.createChar(1, START_DIV_1_OF_1);
  this->lcd.createChar(2, DIV_0_OF_2);
  this->lcd.createChar(3, DIV_1_OF_2);
  this->lcd.createChar(4, DIV_2_OF_2);
  this->lcd.createChar(5, END_DIV_0_OF_1);
  this->lcd.createChar(6, END_DIV_1_OF_1);
}

void KeypadShield::loop(Victron victron) {

  switch (this->getButton()) {
    case RIGHT:
    case UP:
      this->currentView += 1;
      if (this->currentView > ERR)
        this->currentView = PPV;
      break;
    case LEFT:
    case DOWN:
      this->currentView -= 1;
      if (this->currentView < PPV)
        this->currentView = ERR;
      break;
    case SELECT:
      this->turnDisplayOff();
      break;
    case NONE:
    default:
      break;
  };

  switch (this->currentView) {
    case PPV:
      this->displayPvWatt(victron);
      break;
    case VPV:
      this->displayPvVolt(victron);
      break;
    case VB:
      this->displayBVolt(victron);
      break;
    case ERR:
      this->displayErrCode(victron);
      break;
  }

  this->displayChargeState(victron);

  if (millis() - lastBtnClick > 5 * 60 * 1000)
    this->turnDisplayOff();
}
