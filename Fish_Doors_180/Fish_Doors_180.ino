#include <Servo.h>
#include "Arduino.h"

Servo door1, door2;
int door1Pos, door2Pos;
int minPos = 750, maxPos = 2250;
String read_string;


void setup() {
  door1.attach(9);
  door2.attach(10);
  door1.writeMicroseconds(minPos);
  door1Pos = minPos;
  delay(15);
  door2.writeMicroseconds(maxPos);
  door2Pos = maxPos;
  delay(15);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  //Serial.println("Arduino ready");
}

void loop() {
  /*  if (Serial.available() > 0) {
      int input = Serial.read();
      if (input == 49) {	//press 1 to toggle door 1
        toggleDoor(1, door1Pos);
      }
      else if (input == 50) {	//press 2 to toggle door 2
        toggleDoor(2, door2Pos);
      }
    }*/
  //String read_string;
  while (Serial.available() > 0) {
    delay(30);
    char c = Serial.read();
    read_string += c;
  }
  if (read_string.length() > 0) {
    Serial.println(read_string);
    if (read_string == "LO" && door1Pos == minPos) {
      openDoor(1);
    }
    else if (read_string == "LC" && door1Pos == maxPos) {
      closeDoor(1);
    }
    else if (read_string == "RC" && door2Pos == minPos) {
      openDoor(2);
    }
    else if (read_string == "RO" && door2Pos == maxPos) {
      closeDoor(2);
    }
  }
  read_string ="";
}

String takeInput() {
  String read_string;
  while (Serial.available() > 0) {
    delay(30);
    char c = Serial.read();
    read_string += c;
  }
  return read_string;
}


void openDoor(int door) {
  // Serial.print("Opening door ");
  // Serial.println(door);
  if (door == 1) {
    for (int pos = minPos; pos <= maxPos; pos += 1) {
      door1.writeMicroseconds(pos);
      door1Pos = pos;
      delay(5);
      door1Pos = 1;
    }
  }
  else {
    for (int pos = minPos; pos <= maxPos; pos += 1) {
      door2.writeMicroseconds(pos);
      door2Pos = pos;
      delay(5);
      door2Pos = 1;
    }
  }
  // Serial.println("Done");
}

void closeDoor(int door) {
  // Serial.print("Closing door ");
  // Serial.println(door);
  if (door == 1) {
    for (int pos = maxPos; pos >= minPos; pos -= 1) {
      door1.writeMicroseconds(pos);
      door1Pos = pos;
      delay(5);
      door1Pos = 0;
    }
  }
  else {
    for (int pos = maxPos; pos >= minPos; pos -= 1) {
      door2.writeMicroseconds(pos);
      door2Pos = pos;
      delay(5);
      door2Pos = 0;
    }
  }
  // Serial.println("Done");
}

void toggleDoor(int door, int doorPos) {
  if (doorPos == 0) {
    openDoor(door);
  }
  else {
    closeDoor(door);
  }
}
