
#include "Remotes.h"


void Remotes::setup() {
  pinMode(pin_interrupt, INPUT_PULLUP);
  mySwitch = RCSwitch();
  mySwitch.enableReceive(digitalPinToInterrupt(pin_interrupt));

  this->pwm = Adafruit_PWMServoDriver();
  this->pwm.begin();
  this->pwm.setOscillatorFrequency(27000000);
  this->pwm.setPWMFreq(1000);
}

boolean Remotes::arrayContain(unsigned long testArray[], unsigned long value) {
  byte arrayLength = sizeof(testArray);
  for (byte i; i < arrayLength; i++)
    if ( value == testArray[i] )
      return true;
  return false;
}

unsigned long Remotes::getButton() {
  if (mySwitch.available()) {
    unsigned long id = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    if (id == this->lastBtnClick && millis() - this->lastTimeBtnClick < 200)
      return 0;

    this->lastTimeBtnClick = millis();
    this->lastBtnClick = id;
    return id;
  }
  return 0;
}

void Remotes::setLight1(int value) {
  if (value < 0)
    value = 0;
  else if (value > this->lightLevels)
    value = this->lightLevels;

  if (this->levelLight1 == value)
    return;

  int pwmValue = map(value, 0, lightLevels, 0, 4096);
  this->pwm.setPWM(0, pwmValue, 4096 - pwmValue);

  this->levelLight1 = value;
}

void Remotes::setLight2(int value) {
  if (value < 0)
    value = 0;
  else  if (value > this->lightLevels)
    value = this->lightLevels;

  if (this->levelLight2 == value)
    return;
  this->levelLight2 = value;
}

void Remotes::setLight3(int value) {
  if (value < 0)
    value = 0;
  else if (value > this->lightLevels)
    value = this->lightLevels;

  if (this->levelLight3 == value)
    return;
  this->levelLight3 = value;
}

void Remotes::loop(KeypadShield &keypadShield) {
  unsigned long id = this->getButton();
  if (id > 0) {
    if (this->arrayContain(ALL_LIGHTS_ON, id) == true) {
      this->setLight1(this->lightLevels);
      this->setLight2(this->lightLevels);
      this->setLight3(this->lightLevels);
      keypadShield.writeTmpMessage("ALL LIGHTS ON");
    }
    else if (this->arrayContain(ALL_LIGHTS_OFF, id) == true) {
      this->setLight1(0);
      this->setLight2(0);
      this->setLight3(0);
      keypadShield.writeTmpMessage("ALL LIGHTS OFF");
    }

    else if (this->arrayContain(LIGHT_1_ON, id) == true) {
      if (this->levelLight1 > 0) {
        this->setLight1(this->levelLight1 + 1);
      }
      else {
        this->setLight1(this->lightLevels);
      }
      keypadShield.writeTmpMessage("LIGHT 1 ON (" + String(this->levelLight1) + "/" + String(this->lightLevels) + ")");

    }
    else if (this->arrayContain(LIGHT_1_OFF, id) == true) {
      keypadShield.writeTmpMessage("LIGHT 1 OFF");
      this->setLight1(0);
    }
    else if (this->arrayContain(LIGHT_2_ON, id) == true ) {
      if (this->levelLight2 > 0) {
        this->setLight2(this->levelLight2 + 1);
      }
      else {
        this->setLight2(this->lightLevels);
      }
      keypadShield.writeTmpMessage("LIGHT 2 ON (" + String(this->levelLight2) + "/" + String(this->lightLevels) + ")");

    }
    else if (this->arrayContain(LIGHT_2_OFF, id) == true) {
      keypadShield.writeTmpMessage("LIGHT 2 OFF");
      this->setLight2(0);
    }
    else if (this->arrayContain(LIGHT_3_ON, id) == true) {
      if (this->levelLight3 > 0) {
        this->setLight3(this->levelLight3 + 1);
      }
      else {
        this->setLight3(this->lightLevels);
      }
      keypadShield.writeTmpMessage("LIGHT 3 ON (" + String(this->levelLight3) + "/" + String(this->lightLevels) + ")");
    }
    else if (this->arrayContain(LIGHT_3_OFF, id) == true) {
      keypadShield.writeTmpMessage("LIGHT 3 OFF");
      this->setLight3(0);
    }

    else if (this->arrayContain(LIGHT_1_DEC, id) == true) {
      this->setLight1(this->levelLight1 - 1);
      keypadShield.writeTmpMessage("LIGHT 1    (" + String(this->levelLight1) + "/" + String(this->lightLevels) + ")");
    }
    else if (this->arrayContain(LIGHT_1_INC, id) == true) {
      this->setLight1(this->levelLight1 + 1);
      keypadShield.writeTmpMessage("LIGHT 1    (" + String(this->levelLight1) + "/" + String(this->lightLevels) + ")");
    }

    else if (this->arrayContain(LIGHT_2_DEC, id) == true) {
      this->setLight2(this->levelLight2 - 1);
      keypadShield.writeTmpMessage("LIGHT 2    (" + String(this->levelLight2) + "/" + String(this->lightLevels) + ")");
    }
    else if (this->arrayContain(LIGHT_2_INC, id) == true) {
      this->setLight2(this->levelLight2 + 1);
      keypadShield.writeTmpMessage("LIGHT 2    (" + String(this->levelLight2) + "/" + String(this->lightLevels) + ")");
    }

    else if (this->arrayContain(LIGHT_3_DEC, id) == true) {
      this->setLight3(this->levelLight3 - 1);
      keypadShield.writeTmpMessage("LIGHT 3    (" + String(this->levelLight3) + "/" + String(this->lightLevels) + ")");
    }
    else if (this->arrayContain(LIGHT_3_INC, id) == true) {
      this->setLight3(this->levelLight3 + 1);
      keypadShield.writeTmpMessage("LIGHT 3    (" + String(this->levelLight3) + "/" + String(this->lightLevels) + ")");
    }
    else {
      keypadShield.writeTmpMessage("BTN NOT ASSIGNED");
    }
  }
}
