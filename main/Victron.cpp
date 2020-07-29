
#include "Victron.h"

//<Newline><Field-Label><Tab><Field-Value>
// \r\nVPV\t1337

boolean Victron::fetch() {

  //Serial.println("Victron::fetch()");
  String message = "";
  for (int i = 0; i < 200; i++) {
    if (victronSerial.available() > 0)
      message += (char) victronSerial.read();
    else
      break;
  }

  if (message.length() == 0) {
    return true;
  }

  /*
    Serial.println("msg:");
    Serial.println(message);
  */

  boolean hasError = false;
  String fieldLabel = "";
  boolean readValue = false;
  float bufferValue = 0;
  int comma = -1;
  boolean isNegativeValue = false;

  for (int i = 0; i <  message.length(); i++) {
    char c = message.charAt(i);

    if (c == '\r') {

    }
    else if (c == '\n' && fieldLabel.length() > 0 && readValue) { //
      if (comma > 0)
        bufferValue = bufferValue / pow(10, comma);
      if (isNegativeValue)
        bufferValue = -bufferValue;

      /*
            Serial.print(bufferName);
            Serial.println(": " + String(bufferValue));
      */

      if (hasError == false) {
        if (fieldLabel == "V")
          this->bVolt = bufferValue / 1000.0;
        else if (fieldLabel == "I")
          this->bAmp = bufferValue / 1000.0;
        else if (fieldLabel == "VPV")
          this->pvVolt = bufferValue / 1000.0;
        else if (fieldLabel == "PPV")
          this->pvWatt = bufferValue;
        else if (fieldLabel == "H20")
          this->yeldToday = bufferValue;
        else if (fieldLabel == "H22")
          this->yeldYesterday = bufferValue;
        else if (fieldLabel == "CS")
          this->chargeState = (int) bufferValue;
        else if (fieldLabel == "ERR")
          this->errCode = (int) bufferValue;
      } else {
        /*
          Serial.println("has error");
          Serial.print(bufferName);
          Serial.println(": " + String(bufferValue));
        */
      }

      hasError = false;
      fieldLabel = "";
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
      if (fieldLabel == "V" || fieldLabel == "VPV" || fieldLabel == "PPV" || fieldLabel == "CS" || fieldLabel == "ERR") {
        if (c == '-') {
          isNegativeValue = true;
        } else if ((c == '.' || c == ',') && comma < 0) {
          comma = 0;
        }
        else if (c >= '0' && c <= '9') {
          if (comma >= 0)
            comma += 1;
          bufferValue = (bufferValue * 10) + (int)(c - 48);
        } else
          hasError = true;
      }

      //text value
      //TODO
    }
    else  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
      fieldLabel += c;
    }
  }
  return hasError;
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

int Victron::getChargeStateCode() {
  return this->chargeState;
}

String Victron::getChargeStateLabel() {
  const String labels[6] = {"Off", "Low power", "Fault", "Bulk", "Absorption", "Float"}; // 0,1,2,3,4,5, (9=Inverting)
  if (this->chargeState < 6)
    return labels[this->chargeState];
}

int Victron::computeStateOfChargeFromVoltage() {
  const float voltages[11][2] = {
    {10.5, 0},
    {11.31, 10},
    {11.58, 20},
    {11.75, 30},
    {11.9, 40},
    {12.06, 50},
    {12.2, 60},
    {12.32, 70},
    {12.42, 80},
    {12.5, 90},
    {12.6, 100}
  };

  for (int i = 10; i >= 0; i--) {
    if (this->bVolt > voltages[i][0]) {
      if (i == 10)
        return 100;
      return (int) map(bVolt, voltages[i][0], voltages[i + 1][0], voltages[i][1], voltages[i + 1][1]);
    }
  }
  return 0;
}


int Victron::getBatteryStateOfCharge() {
  int result = this->computeStateOfChargeFromVoltage();
  switch (this->getChargeStateCode()) {
    case 0: // Off
    default:
      break;

    case 3: //Bulk (batterie partiellement chargée)
      break;

    case 4: //Absorption (batterie chargée à 80 % ou plus)
      if (result < 80)
        result = 80;
      break;

    case 5: //Float
      result = 100;
      break;
  }
  return result;
}

int Victron::getErrCode() {
  return this->errCode;
}

String Victron::getErrLabel() {
  const int errCodes[14] = {0, 2, 17, 18, 19, 20, 21, 26, 33, 34, 38, 116, 117, 119};
  const String errLabels[14] = {
    "No error",
    "Battery voltage too high",
    "Charger temperature too high",
    "Charger over current",
    "Charger current reversed",
    "Bulk time limit exceeded",
    "Current sensor issue",
    "Terminals overheated",
    "Input voltage too high",
    "Input current too high",
    "Input shutdown",
    "Factory calibration data lost",
    "Invalid/incompatible firmware",
    "User settings invalid"
  };

  for (int i = 0; i < 14; i++) {
    if (    errCodes[i] == this->errCode)
      return errLabels[i];
  }
}

void Victron::setup() {
  victronSerial.begin(19200);
  victronSerial.flush();
}

void Victron::loop() {
  boolean error;
  int i = 0;
  do {
    error = this->fetch();
    i++;
  } while (error && i <= 10);
}
