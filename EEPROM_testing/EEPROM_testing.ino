#include "Arduino.h"
#include "EEPROM.h"

int addr = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int val = 27;
  EEPROM.write(addr, val);
  addr+=1;
  if (addr == EEPROM.length()) {
      addr=0;
  }
  delay(1000);
}
