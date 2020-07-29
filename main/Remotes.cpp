
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

void Remotes::setLight(int lightId, int level, boolean showMessage = true) {

  if (level < 0)
    level = 0;
  else if (level > this->lightLevels)
    level = this->lightLevels;

  int *currentLevel;
  int pin;
  switch (lightId) {
    case 1:
      currentLevel = &this->levelLight1;
      pin = this->pinLight1;
      break;
    case 2:
      currentLevel = &this->levelLight2;
      pin = this->pinLight2;
      break;
    case 3:
      currentLevel = &this->levelLight3;
      pin = this->pinLight3;
      break;
    default:
      this->keypadShield->writeTmpMessage("unknow light id: " + String(lightId), 1000);
      return;
      break;
  }

  if (*currentLevel == level)
    return;

  //int pwmValue = map(value, 0, this->lightLevels, 0, 4096);
  int pwmValue = this->levelValues[level];

  //Serial.print("pwmValue: ");
  //Serial.println(pwmValue);

  this->pwm.setPin(pin, pwmValue);
  *currentLevel = level;

  if (showMessage)
    this->keypadShield->writeTmpMessage("LIGHT " + String(lightId ) + "    (" + String(level) + "/" + String(this->lightLevels) + ")", 1000);
}

void Remotes::setAllLights(int level, boolean showMessage = true) {

  this->setLight(1, level, false);
  this->setLight(2, level, false);
  this->setLight(3, level, false);

  if (showMessage)
    this->keypadShield->writeTmpMessage("ALL LIGHTS (" + String(level) + "/" + String(this->lightLevels) + ")", 1000);
}


void Remotes::increaseAllLights( boolean showMessage = true) {
  this->setLight(1, this->levelLight1 + 1, false);
  this->setLight(2, this->levelLight2 + 1, false);
  this->setLight(3, this->levelLight3 + 1, false);
  if (showMessage)
    this->keypadShield->writeTmpMessage("INCREASE ALL LIGHTS", 1000);
}

void Remotes::decreaseAllLights( boolean showMessage = true) {
  this->setLight(1, this->levelLight1 - 1, false);
  this->setLight(2, this->levelLight2 - 1, false);
  this->setLight(3, this->levelLight3 - 1, false);
  if (showMessage)
    this->keypadShield->writeTmpMessage("DECREASE ALL LIGHTS", 1000);
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
      this->setAllLights(this->levelOn);

    else if (this->arrayContain(ALL_OFF, id) == true)
      this->setAllLights(0);

    else if (this->arrayContain(A_1_ON, id) == true) {
      if (this->levelLight1 > 0)
        this->setLight(1, this->levelLight1 + 1);
      else
        this->setLight(1, this->levelOn);
    }

    else if (this->arrayContain(A_1_OFF, id) == true)
      this->setLight(1, 0);

    else if (this->arrayContain(A_2_ON, id) == true ) {
      if (this->levelLight2 > 0)
        this->setLight(2, this->levelLight2 + 1);
      else
        this->setLight(2, this->levelOn);
    }

    else if (this->arrayContain(A_2_OFF, id) == true)
      this->setLight(2, 0);

    else if (this->arrayContain(A_3_ON, id) == true) {
      if (this->levelLight3 > 0)
        this->setLight(3, this->levelLight3 + 1);
      else
        this->setLight(3, this->levelOn);
    }

    else if (this->arrayContain(A_3_OFF, id) == true)
      this->setLight(3, 0);

    else if (this->arrayContain(A_4_ON, id) == true)
      this->decreaseAllLights();

    else if (this->arrayContain(A_4_OFF, id) == true)
      this->increaseAllLights();

    else if (this->arrayContain(B_1_ON, id) == true)
      this->setLight(1, this->levelLight1 - 1);

    else if (this->arrayContain(B_1_OFF, id) == true)
      this->setLight(1, this->levelLight1 + 1);

    else if (this->arrayContain(B_2_ON, id) == true)
      this->setLight(2, this->levelLight2 - 1);
    else if (this->arrayContain(B_2_OFF, id) == true)
      this->setLight(2, this->levelLight2 + 1);

    else if (this->arrayContain(B_3_ON, id) == true)
      this->setLight(3, this->levelLight3 - 1);

    else if (this->arrayContain(B_3_OFF, id) == true)
      this->setLight(3, this->levelLight3 + 1);

    else if (this->arrayContain(B_4_ON, id) == true)
      this->decreaseAllLights();

    else if (this->arrayContain(B_4_OFF, id) == true)
      this->increaseAllLights();

    else if (this->arrayContain(LIGHT_SWITCH_LEFT, id) == true) {
      if (this->levelLight1 > 0)
        this->setLight(1, 0);
      else
        this->setLight(1, this->levelOn);
    }
    else if (this->arrayContain(LIGHT_SWITCH_RIGHT, id) == true) {
      if (this->levelLight2 > 0 || this->levelLight3 > 0) {
        this->setLight(2, 0);
        this->setLight(3, 0);
      }
      else {
        this->setLight(2, this->levelOn);
        this->setLight(3, this->levelOn);
      }
    }
    else
      this->keypadShield->writeTmpMessage("BTN NOT ASSIGNED", 1000);
  }

}
