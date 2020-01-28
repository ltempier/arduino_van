
#include "Victron.h"

void Victron::fetch() {
  while (Serial.available() > 0) {
    char c = Serial.read();
  }
}
void Victron::setup() {
  Serial.begin(19200);
}
float Victron::getPvWatt() {
  return this->pvWatt;
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

}
