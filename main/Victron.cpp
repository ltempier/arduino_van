
#include "Victron.h"

void Victron::fetch() {

  //Serial.println("Victron::fetch()");
  String message = "";
  for (int i = 0; i < 200; i++) {
    if (victronSerial.available() > 0)
      message += (char) victronSerial.read();
    else
      break;
  }

  if (message.length() == 0)
    return;

  /*
    Serial.println("msg:");
    Serial.println(message);
  */
  
  String bufferName = "";
  boolean readValue = false;
  float bufferValue = 0;
  int comma = -1;
  boolean isNegativeValue = false;
  boolean hasError = false;

  for (int i = 0; i <  message.length(); i++) {
    char c = message.charAt(i);
    if (c == '\r') {
    }
    else if (c == '\n' && bufferName.length() > 0 && readValue) { //
      if (comma > 0)
        bufferValue = bufferValue / pow(10, comma);
      if (isNegativeValue)
        bufferValue = -bufferValue;

      /*
            Serial.print(bufferName);
            Serial.println(": " + String(bufferValue));
      */

      if (hasError == false) {
        if (bufferName == "V")
          this->bVolt = bufferValue / 1000.0;
        if (bufferName == "VPV")
          this->pvVolt = bufferValue / 1000.0;
        if (bufferName == "PPV")
          this->pvWatt = bufferValue;
        if (bufferName == "CS")
          this->chargeState = (int) bufferValue;
        if (bufferName == "ERR")
          this->errCode =  (int) bufferValue;
      } else {
        /*
          Serial.println("has error");
          Serial.print(bufferName);
          Serial.println(": " + String(bufferValue));
        */
      }

      hasError = false;
      bufferName = "";
      readValue = false;
      isNegativeValue = false;
    }
    else if (c == '\t') {
      bufferValue = 0;
      comma = -1;
      readValue = true;
      isNegativeValue = false;
    }
    else if (readValue) {

      // muneric value
      if (bufferName == "V" || bufferName == "VPV" || bufferName == "PPV" || bufferName == "CS" || bufferName == "ERR") {
        if (c == '-') {
          isNegativeValue = true;
        } else if ((c == '.' || c == ',') && comma <= 0) {
          comma = 0;
        }
        else if (c >= '0' && c <= '9') {
          if (comma >= 0)
            comma += 1;
          bufferValue = (bufferValue * 10) + (int)(c - 48);
        } else
          hasError = true;
      }


    }
    else  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
      bufferName += c;
    }
  }
}


void Victron::setPvWatt(float value) {
  if (this->pvWatt = -1337)
    this->pvWatt = value;
  else if (value > this->pvWatt * 0.5 && value < this->pvWatt * 1.5)
    this->pvWatt = value;
}

float Victron::getPvWatt() {
  return this->pvWatt;
}


void Victron::setBVolt(float value) {
  if (this->bVolt = -1337)
    this->bVolt = value;
  else if (value > this->bVolt * 0.5 && value < this->bVolt * 1.5)
    this->bVolt = value;
}

float Victron::getBVolt() {
  return this->bVolt;
}


void Victron::setPvVolt(float value) {
  if (this->pvVolt = -1337)
    this->pvVolt = value;
  else if (value > this->pvVolt * 0.5 && value < this->pvVolt * 1.5)
    this->pvVolt = value;
}

float Victron::getPvVolt() {
  return this->pvVolt;
}

void Victron::setChargeState(int value) {
  if (this->chargeState = -1337)
    this->chargeState = value;
  else if (value > this->chargeState - 2 && value < this->chargeState + 2)
    this->chargeState = value;
}

int Victron::getChargeStatePercent() {
  return map(this->chargeState, 0, 9, 0, 100);
}
int Victron::getErrCode() {
  return this->errCode;
}

void Victron::setup() {
  victronSerial.begin(19200);
  victronSerial.flush();
}

void Victron::loop() {
  this->fetch();
}
