#include <Servo.h>
#include "Arduino.h"
#include "EEPROM.h"

Servo door1, door2;

int door1pos = 0, door2pos = 0; //door position, defaults to down

int door1OpenDelay = 3150, door1CloseDelay = 2650, door2OpenDelay = 2900, door2CloseDelay = 3300;

void setup() {
  door1OpenDelay = EEPROM.read(3);
  Serial.println(door1OpenDelay);
  Serial.println(door1OpenDelay, DEC);
  door1CloseDelay = EEPROM.read(1);
  door2OpenDelay = EEPROM.read(4);
  door2CloseDelay = EEPROM.read(2);
  door1.attach(9);
  door2.attach(10);
  door1.write(90);
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
    if (signalIn == 49) {
      toggleDoor(1, door1pos);
    }
    else if (signalIn == 50) {
      toggleDoor(2, door2pos);
    }
    else if (signalIn == 51) {
      toggleBothDoors(door1pos, door2pos);
    }
    else if (signalIn == 105) { //press i for calibrating door 1 open
      door1OpenDelay = calibrate(1, 1);
    }
    else if (signalIn == 107) { //press k for calibrating door 1 close
      door1CloseDelay = calibrate(1, 0);
    }
    else if (signalIn == 111) { //press o for calibrating door 2 open
      door2OpenDelay = calibrate(2, 1);
    }
    else if (signalIn == 108) { //press l for calibrating door 2 close
      door2CloseDelay = calibrate(2, 0);
    }
    else if (signalIn == 99) {
      Serial.println("Calibration:");
      Serial.println("i | door 1 open");
      Serial.println("k | door 1 close");
      Serial.println("o | door 2 open");
      Serial.println("l | door 2 close");
    }
  }
}

void openDoor(int door) {
  if (door == 1) {
    Serial.println("Opening door 1");
    door1.write(88);
    delay(door1OpenDelay);
    door1.write(90);
    door1pos = 1;
    Serial.println("Done");
  }
  else if (door == 2) {
    Serial.println("Opening door 2");
    door2.write(92);
    delay(door2OpenDelay);
    door2.write(90);
    door2pos = 1;
    Serial.println("Done");
  }
  else {
    Serial.println("Opening both doors");
    door1.write(88);
    door2.write(92);
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
    door1.write(92);
    delay(door1CloseDelay);
    door1.write(90);
    door1pos = 0;
    Serial.println("Done");
  }
  else if (door == 2) {
    door2.write(88);
    Serial.println("Closing door 2");
    delay(door2CloseDelay);
    door2.write(90);
    door2pos = 0;
    Serial.println("Done");
  }
  else {
    Serial.println("Closing both doors");
    door1.write(92);
    door2.write(88);
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

void toggleDoor(int door, int doorpos) {
  if (doorpos == 0) {
    openDoor(door);
  }
  else {
    closeDoor(door);
  }
}

void toggleBothDoors (int door1pos, int door2pos) {
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
    door1.write(92 - (4 * dir));
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
    door2.write(88 + (4 * dir));
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
  return output;
  EEPROM.write(door + (2 * dir), output);
}
