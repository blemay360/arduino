#include "Arduino.h"
#include "EEPROM.h"

int addr = 0;
byte value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = EEPROM.read(addr);
  Serial.print(addr);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  addr+=1;
  if (addr == EEPROM.length()) {
    addr =0;
  }
  delay(100);
}
