
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

void setup() {
  mySerial.begin(19200);
  Serial.begin(19200);
  Serial.println("setup done");
}

void loop() {

  Serial.println("******************");
  int l = 0;
  while (mySerial.available()) {
    int inByte = mySerial.read();
    Serial.write(inByte);
    l++;
  }
  Serial.println();
  Serial.print("l: "); Serial.println(l);
}
