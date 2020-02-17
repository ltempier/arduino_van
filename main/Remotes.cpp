
#include "Remotes.h"

byte Remotes::getSavedLightLevel(int address) {
  return EEPROM.read(address);
}

void Remotes::saveLightLevel(int address, byte value) {
  EEPROM.write(address, (byte) value);
}

boolean Remotes::arrayContain(unsigned long testArray[], unsigned long value) {
  for (int i; i < 2; i++)
    if ( value == testArray[i] )
      return true;
  return false;
}

unsigned long Remotes::getButton() {
  if (mySwitch.available()) {
    unsigned long id = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    if (id == this->lastBtnClick && millis() - this->lastTimeBtnClick < 500)
      return 0;

    this->lastTimeBtnClick = millis();
    this->lastBtnClick = id;
    return id;
  }
  return 0;
}

unsigned long Remotes::getButton(unsigned int attempts) {
  for (int i = 0 ; i < attempts; i++) {
    unsigned long id = this->getButton();
    if (id > 0)
      return id;
    delay(50);
  }
  return 0;
}

void Remotes::setLight1(int value, boolean showMessage = true) {
  if (value < 0)
    value = 0;
  else if (value > this->lightLevels)
    value = this->lightLevels;

  if (this->levelLight1 == value)
    return;

  int pwmValue = map(value, 0, this->lightLevels, 0, 4096);
  this->pwm.setPin(0, pwmValue);

  this->levelLight1 = value;
  //this->saveLightLevel(0, value);

  if (showMessage)
    this->keypadShield->writeTmpMessage("LIGHT 1    (" + String(value) + "/" + String(this->lightLevels) + ")", 1000);
}

void Remotes::setLight2(int value, boolean showMessage = true) {
  if (value < 0)
    value = 0;
  else  if (value > this->lightLevels)
    value = this->lightLevels;

  if (this->levelLight2 == value)
    return;

  int pwmValue = map(value, 0, this->lightLevels, 0, 4096);
  this->pwm.setPin(1, pwmValue);

  this->levelLight2 = value;
  //this->saveLightLevel(1, value);

  if (showMessage)
    this->keypadShield->writeTmpMessage("LIGHT 2    (" + String(this->levelLight2) + "/" + String(this->lightLevels) + ")", 1000);
}

void Remotes::setLight3(int value, boolean showMessage = true) {
  if (value < 0)
    value = 0;
  else if (value > this->lightLevels)
    value = this->lightLevels;
  if (this->levelLight3 == value)
    return;

  int pwmValue = map(value, 0, this->lightLevels, 0, 4096);
  this->pwm.setPin(2, pwmValue);

  this->levelLight3 = value;
  //this->saveLightLevel(2, value);

  if (showMessage)
    this->keypadShield->writeTmpMessage("LIGHT 3    (" + String(this->levelLight3) + "/" + String(this->lightLevels) + ")", 1000);
}

void Remotes::setAllLights(int value, boolean showMessage = true) {

  this->setLight1(value, false);
  this->setLight2(value, false);
  this->setLight3(value, false);

  if (showMessage)
    this->keypadShield->writeTmpMessage("ALL LIGHTS (" + String(value) + "/" + String(this->lightLevels) + ")", 1000);
}

void Remotes::setup(KeypadShield *keypadShield) {

  this->keypadShield = keypadShield;

  pinMode(pin_interrupt, INPUT_PULLUP);
  this->mySwitch = RCSwitch();
  this->mySwitch.enableReceive(digitalPinToInterrupt(pin_interrupt));

  this->pwm = Adafruit_PWMServoDriver(0x40);
  this->pwm.begin();
  this->pwm.setOutputMode(true);

  //this->pwm.setOscillatorFrequency(27000000);
  this->pwm.setPWMFreq(1000);

  this->setAllLights(0);
  /*
    this->setLight1(this->getLightLevel(0));
    this->setLight2(this->getLightLevel(1));
    this->setLight3(this->getLightLevel(2));
  */
}

void Remotes::loop() {
  unsigned long id = this->getButton(3);
  if (id > 0) {

    //Serial.print("click remote: ");
    //Serial.println(id);

    if (this->arrayContain(ALL_ON, id) == true)
      this->setAllLights(this->lightLevels);

    else if (this->arrayContain(ALL_OFF, id) == true)
      this->setAllLights(0);

    else if (this->arrayContain(A_1_ON, id) == true) {
      if (this->levelLight1 > 0)
        this->setLight1(this->levelLight1 + 1);
      else
        this->setLight1(this->lightLevels);
    }

    else if (this->arrayContain(A_1_OFF, id) == true)
      this->setLight1(0);

    else if (this->arrayContain(A_2_ON, id) == true ) {
      if (this->levelLight2 > 0)
        this->setLight2(this->levelLight2 + 1);
      else
        this->setLight2(this->lightLevels);
    }

    else if (this->arrayContain(A_2_OFF, id) == true)
      this->setLight2(0);

    else if (this->arrayContain(A_3_ON, id) == true) {
      if (this->levelLight3 > 0)
        this->setLight3(this->levelLight3 + 1);
      else
        this->setLight3(this->lightLevels);
    }

    else if (this->arrayContain(A_3_OFF, id) == true)
      this->setLight3(0);

    else if (this->arrayContain(B_1_ON, id) == true)
      this->setLight1(this->levelLight1 - 1);

    else if (this->arrayContain(B_1_OFF, id) == true)
      this->setLight1(this->levelLight1 + 1);

    else if (this->arrayContain(B_2_ON, id) == true)
      this->setLight2(this->levelLight2 - 1);
    else if (this->arrayContain(B_2_OFF, id) == true)
      this->setLight2(this->levelLight2 + 1);

    else if (this->arrayContain(B_3_ON, id) == true)
      this->setLight3(this->levelLight3 - 1);

    else if (this->arrayContain(B_3_OFF, id) == true)
      this->setLight3(this->levelLight3 + 1);

    else if (this->arrayContain(LIGHT_SWITCH_LEFT, id) == true) {
      if (this->levelLight1 > 0)
        this->setLight1(0);
      else
        this->setLight1(this->lightLevels);
    }
    else if (this->arrayContain(LIGHT_SWITCH_RIGHT, id) == true) {
      if (this->levelLight2 > 0)
        this->setLight2(0);
      else
        this->setLight2(this->lightLevels);
    }
    else
      this->keypadShield->writeTmpMessage("BTN NOT ASSIGNED", 1000);
  }
}
