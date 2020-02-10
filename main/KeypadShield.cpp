
#include "KeypadShield.h"

void KeypadShield::turnDisplayOn() {
  digitalWrite(pin_LCD_BL, HIGH);
  this->lastTimeTurnDisplayOn = millis();
  this->backlightState = true;
}

void KeypadShield::turnDisplayOff() {
  digitalWrite(pin_LCD_BL, LOW);
  this->backlightState = false;
}

int KeypadShield::getButton() {
  static const int values[] = { 50, 150, 300, 500, 750 };
  int value = analogRead(pin_button);
  if (value > values[4] || millis() - this->lastTimeBtnClick < 300)
    return NONE;

  this->lastTimeBtnClick = millis();

  if (!this->backlightState) {
    this->turnDisplayOn(); //SET this->backlightState = TRUE
    return NONE;
  }

  this->turnDisplayOn();

  for (int i = 0; i < 5; i++) {
    if (value < values[i])
      return i;
  }
}

void KeypadShield::writeTopLine(String topLine, boolean force = false) {
  if (!force) {
    if (topLine.equals(this->currentTopLine))
      return;
    if (this->tmpDisplayDuration > 0 && (millis() - this->lastTimeTmpDisplay)  < this->tmpDisplayDuration) {
      return;
    } else
      this->tmpDisplayDuration = 0;
  }

  while (topLine.length() < 16)
    topLine += " ";
  if (topLine.length() > 16)
    topLine = topLine.substring(0, 16);

  this->lcd.setCursor(0, 0);
  this->lcd.print(topLine);
  this->currentTopLine = topLine;
}

void KeypadShield::writeTmpMessage(String msg, int tmpDisplayDuration = 3000) {
  this->lastTimeTmpDisplay = millis();
  this->tmpDisplayDuration = tmpDisplayDuration;
  this->writeTopLine(msg, true);
  this->turnDisplayOn();
}


void KeypadShield::displayPvWatt(Victron victron, boolean force = false) {
  String topLine = "Ppv: ";
  String sPvWatt = String(victron.getPvWatt(), 2);
  sPvWatt += " W";

  byte appendSpaces =  16 - (topLine.length() + sPvWatt.length());
  for (byte i = 0 ; i < appendSpaces; i++)
    topLine += " ";

  this->writeTopLine(topLine + sPvWatt, force);
}

void KeypadShield::displayPvVolt(Victron victron, boolean force = false) {
  String topLine = "Vpv: ";
  String sPvVolt = String(victron.getPvVolt(), 2);
  sPvVolt += " V";

  byte appendSpaces =  16 - (topLine.length() + sPvVolt.length());
  for (byte i = 0 ; i < appendSpaces; i++)
    topLine += " ";
  this->writeTopLine(topLine + sPvVolt, force);

}

void KeypadShield::displayBVolt(Victron victron, boolean force = false) {
  String topLine = "Vb: ";
  String sBVolt = String(victron.getBVolt(), 2);
  sBVolt += " V";

  byte appendSpaces =  16 - (topLine.length() + sBVolt.length());
  for (byte i = 0 ; i < appendSpaces; i++)
    topLine += " ";
  this->writeTopLine(topLine + sBVolt, force);

}

void KeypadShield::displayErrCode(Victron victron, boolean force = false) {
  String topLine = "Err: ";
  String sErrCode = String(victron.getErrCode());
  byte appendSpaces =  16 - (topLine.length() + sErrCode.length());
  for (byte i = 0 ; i < appendSpaces; i++)
    topLine += " ";
  this->writeTopLine(topLine + sErrCode, force);
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
  delay(10);
}

void KeypadShield::loop(Victron &victron) {

  int button = this->getButton();
  switch (button) {
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
      this->displayPvWatt(victron, button != NONE);
      this->displayChargeState(victron);

      break;
    case VPV:
      this->displayPvVolt(victron, button != NONE);
      this->displayChargeState(victron);

      break;
    case VB:
      this->displayBVolt(victron, button != NONE);
      this->displayChargeState(victron);

      break;
    case ERR:
      this->displayErrCode(victron, button != NONE);
      this->displayChargeState(victron);
      break;
  }


  if (millis() - this->lastTimeTurnDisplayOn >  60000) // turnDisplayOff after 60 sec
    this->turnDisplayOff();
}
