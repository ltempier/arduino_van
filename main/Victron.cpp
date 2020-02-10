
#include "Victron.h"

void Victron::fetch() {

  String bufferName = "";
  boolean readValue = false;
  float bufferValue = 0;
  int comma = -1;
  boolean isNegativeValue = false;

  unsigned long start = 0;
  while (Serial.available() > 0) {
    start = micros();
    char c = Serial.read();
    if (c == '\n') {

      if (comma >= 0)
        bufferValue = bufferValue / pow(10, comma);
      if (isNegativeValue)
        bufferValue = -bufferValue;

      //Serial.print(bufferName);
      //Serial.println(": " + String(bufferValue));

      if (bufferName == "V")
        this->bVolt = bufferValue / 1000.0;
      if (bufferName == "VPV")
        this->pvVolt = bufferValue / 1000.0;
      if (bufferName == "PPV")
        this->pvWatt = bufferValue;
      if (bufferName == "CS")
        this->chargeState =  bufferValue;
      if (bufferName == "ERR")
        this->errCode =  bufferValue;

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
      if (bufferName == "V" || bufferName == "VPV" || bufferName == "PPV" || bufferName == "CS" || bufferName == "ERR") {
        if (c == '-') {
          isNegativeValue = true;
        } else if ((c == '.' || c == ',') && comma == 0) {
          comma = 0;
        }
        else if (c >= '0' && c <= '9') {
          if (comma >= 0)
            comma += 1;
          bufferValue = (bufferValue * 10) + (int)(c - 48);
        }
      }
    }
    else  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
      bufferName += c;
    }

    int waitMicro = this->comDelayMicros - (micros() - start);
    delayMicroseconds((waitMicro > 0 && waitMicro <= this->comDelayMicros) ? waitMicro : 0 );
  }
}

void Victron::setup() {
  Serial.begin(19200);
}

float Victron::getPvWatt() {
  return this->pvWatt;
}

void Victron::fillHistPvWatt(float bufferHistPvWatt[]) {
  for (int i = 0; i < 40; i++)
    bufferHistPvWatt[i] = this->histPvWatt[i];
}

float Victron::getBVolt() {
  return this->bVolt;
}
float Victron::getPvVolt() {
  return this->pvVolt;
}
int Victron::getChargeStatePercent() {
  return map(this->chargeState, 0, 9, 0, 100);
}
int Victron::getErrCode() {
  return this->errCode;
}

void Victron::loop() {
  this->fetch();
}
