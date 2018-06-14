#include <Servo.h>
#include "Arduino.h"
#include "EEPROM.h"

Servo door1, door2;
int doorSpeed = 2;
int doorDir = -1; //change to -1 if direction is backwards

int door1pos = 0, door2pos = 0; //door position, defaults to down
int door1OpenDelay = 3150, door1CloseDelay = 2650, door2OpenDelay = 2900, door2CloseDelay = 3300;

void setup() {
  door1OpenDelay = EEPROM16_read(4);  //Read values from EEPROM for door delays
  door1CloseDelay = EEPROM16_read(0);
  door2OpenDelay = EEPROM16_read(6);
  door2CloseDelay = EEPROM16_read(2);
  door1.attach(9);  //Door 1 is controlled with pin 9
  door2.attach(10); //Door 2 is controlled with pin 10
  door1.write(90);  //Makes sure both servos are stopped
  door2.write(90);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("Ready");
}

void loop() {
  if (Serial.available() > 0) {
    int signalIn = Serial.read();
    if (signalIn == 49) { //press 1 to toggle door 1
      toggleDoor(1, door1pos);
    }
    else if (signalIn == 50) {  //press 2 to toggle door 2
      toggleDoor(2, door2pos);
    }
    else if (signalIn == 51) {  //press 3 to toggle both doors
      toggleBothDoors();
    }
    else if (signalIn == 105) { //press i for calibrating door 1 open
      door1OpenDelay = calibrate(1, 0);
    }
    else if (signalIn == 107) { //press k for calibrating door 1 close
      door1CloseDelay = calibrate(1, 1);
    }
    else if (signalIn == 111) { //press o for calibrating door 2 open
      door2OpenDelay = calibrate(2, 0);
    }
    else if (signalIn == 108) { //press l for calibrating door 2 close
      door2CloseDelay = calibrate(2, 1);
    }
    else if (signalIn == 99) {  //press c to get the calibration codes
      Serial.println("Calibration:");
      Serial.println("i | door 1 open");
      Serial.println("k | door 1 close");
      Serial.println("o | door 2 open");
      Serial.println("l | door 2 close");
    }
    else if (signalIn == 121) { //press y for moving door 1 open
      moveDoor(1,0);
    }
    else if (signalIn == 104) { //press h for moving door 1 close
      moveDoor(1,1);
    }
    else if (signalIn == 117) { //press u for moving door 2 open
      moveDoor(2,0);
    }
    else if (signalIn == 106) { //press j for moving door 2 close
      moveDoor(2,1);
    }
  }
}

void openDoor(int door) {
  if (door == 1) {
    Serial.println("Opening door 1");
    door1.write(90 - (doorDir * doorSpeed));
    delay(door1OpenDelay);
    door1.write(90);
    door1pos = 1;
    Serial.println("Done");
  }
  else if (door == 2) {
    Serial.println("Opening door 2");
    door2.write(90 + (doorDir * doorSpeed));
    delay(door2OpenDelay);
    door2.write(90);
    door2pos = 1;
    Serial.println("Done");
  }
  else {
    Serial.println("Opening both doors");
    door1.write(90 - (doorDir * doorSpeed));
    door2.write(90 + (doorDir * doorSpeed));
    if (door2OpenDelay > door1OpenDelay) {
      delay(door1OpenDelay);
      door1.write(90);
      delay(door2OpenDelay - door1OpenDelay);
      door2.write(90);
      door1pos = 1, door2pos = 1;
      Serial.println("Done");
    }
    else {
      delay(door2OpenDelay);
      door2.write(90);
      delay(door1OpenDelay - door2OpenDelay);
      door1.write(90);
      door1pos = 1, door2pos = 1;
      Serial.println("Done");
    }
  }
}

void closeDoor(int door) {
  if (door == 1) {
    Serial.println("Closing door 1");
    door1.write(90 + (doorDir * doorSpeed));
    delay(door1CloseDelay);
    door1.write(90);
    door1pos = 0;
    Serial.println("Done");
  }
  else if (door == 2) {
    door2.write(90 - (doorDir * doorSpeed));
    Serial.println("Closing door 2");
    delay(door2CloseDelay);
    door2.write(90);
    door2pos = 0;
    Serial.println("Done");
  }
  else {
    Serial.println("Closing both doors");
    door1.write(90 + (doorDir * doorSpeed));
    door2.write(90 - (doorDir * doorSpeed));
    if (door2CloseDelay > door1CloseDelay) {
      delay(door1CloseDelay);
      door1.write(90);
      delay(door2CloseDelay - door1CloseDelay);
      door2.write(90);
      door1pos = 0, door2pos = 0;
      Serial.println("Done");
    }
    else {
      delay(door2CloseDelay);
      door2.write(90);
      delay(door1CloseDelay - door2CloseDelay);
      door1.write(90);
      door1pos = 0, door2pos = 0;
      Serial.println("Done");
    }
  }
}

void moveDoor(int door,int dir) { //function to move doors continuously
  if (door == 1) {
    door1.write((90 - (doorDir * doorSpeed)) - (4 * dir));
    while (Serial.available() <= 0) {
      ;
    }
    door1.write(90);
  }
  else {
    door2.write((90 + (doorDir * doorSpeed)) + (4 * dir));
    while (Serial.available() <= 0) {
      ;
    }
    door2.write(90);
  }
}

void toggleDoor(int door, int doorpos) {
  if (doorpos == 0) {
    openDoor(door);
  }
  else {
    closeDoor(door);
  }
}

void toggleBothDoors()  {
  if (door1pos == 0 && door2pos == 0) {
    openDoor(3);
  }
  else if (door1pos == 1 && door2pos == 1) {
    closeDoor(3);
  }
  else if (door1pos == 1) {
    closeDoor(1);
  }
  else {
    closeDoor(2);
  }
}

int calibrate(int door, int dir) { //dir is 1 for open, 0 for close
  int startTime, endTime;
  if (door == 1) {
    startTime = millis();
    door1.write((90 - (doorDir * doorSpeed)) - (4 * dir));
    while (Serial.available() <= 0) {
      ;
    }
    door1.write(90);
    endTime = millis();
    if (dir == 0) {
      door1pos = 0;
    }
    else {
      door1pos = 1;
    }
  }
  else {
    startTime = millis();
    door2.write((90 + (doorDir * doorSpeed)) + (4 * dir));
    while (Serial.available() <= 0) {
      ;
    }
    door2.write(90);
    endTime = millis();
    if (dir == 0) {
      door2pos = 0;
    }
    else {
      door2pos = 1;
    }
  }
  int output = endTime - startTime;
  Serial.println(output);
  uint8_t address = (door + (2 * dir)) * 2 - 2;
  EEPROM16_write(address, output);
  return output;
}

void EEPROM16_write(uint8_t a, uint16_t b) {
  EEPROM.write(a, lowByte(b));
  EEPROM.write(a + 1, highByte(b));
}

uint16_t EEPROM16_read(uint8_t a) {
  return word(EEPROM.read(a + 1), EEPROM.read(a));
}
