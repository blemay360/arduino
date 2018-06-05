#include <Servo.h>

Servo door1;  
Servo door2;

int pos = 0;    // variable to store the servo position

void setup() {
  door1.attach(9);
  door2.attach(10);
  door1.write(90);
  door2.write(90);
  delay(1000);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  establishContact();
/*  myservo.write(94);
  delay(2800);
  myservo.write(92);
  delay(1000);
  myservo.write(90);
  delay(3500);
  myservo.write(92);
  */
}

void loop() {
  if (Serial.available() > 0) {
    int signalIn = Serial.read();
    if (signalIn == 49) {
      openDoor(1);
    }
    else if (signalIn == 50) {
      openDoor(2);
    }
    else if (signalIn == 51) {
      closeDoor(1);
    }
    else if (signalIn == 52) {
      closeDoor(2);
    }
  }
}



void openDoor(int door) {
  if (door == 1) {
    door1.write(92);
    delay(2440);
    door1.write(90);
  }
  else if (door == 2) {
    door2.write(92);
    delay(2900);
    door2.write(90);
  }
}

void closeDoor(int door) {
  if (door == 1) {
    door1.write(87);
    delay(2025);
    door1.write(90);
  }
  else if (door == 2) {
    door2.write(88);
    delay(3300);
    door2.write(90);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

