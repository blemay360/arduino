#include <Servo.h>

Servo door1;  
Servo door2;

int door1pos = 0; //door 1 position, defaults to down
int door2pos = 0; //door 2 position, defaults to down

int door1OpenDelay = 3150;
int door1CloseDelay = 2650;
int door2OpenDelay = 2900;
int door2CloseDelay = 3300;

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
  //establishContact();
  Serial.println("Ready");
}

void loop() {
  if (Serial.available() > 0) {
    int signalIn = Serial.read();
  /*  if (signalIn == 49) {
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
    }*/
    if (signalIn == 49) {
      toggleDoor(1, door1pos);
    }
    else if (signalIn == 50) {
      toggleDoor(2, door2pos);
    }
    else if (signalIn == 105) { //press i for calibrating door 1 open
      door1OpenDelay = calibrate(1,1);
    }
    else if (signalIn == 107) { //press k for calibrating door 1 close
      door1CloseDelay = calibrate(1,0);
    }
    else if (signalIn == 111) { //press o for calibrating door 2 open
      door2OpenDelay = calibrate(2,1);
    }
    else if (signalIn == 108) { //press l for calibrating door 2 close
      door2CloseDelay = calibrate(2,0);
    }
  }
}



void openDoor(int door) {
  if (door == 1) {
    Serial.println("Opening door 1");
    door1.write(92);
    delay(door1OpenDelay);
    door1.write(90);
    door1pos = 1;
  }
  else if (door == 2) {
    Serial.println("Opening door 2");
    door2.write(92);
    delay(door2OpenDelay);
    door2.write(90);
    door2pos = 1;
  }
}

void closeDoor(int door) {
  if (door == 1) {
    Serial.println("Closing door 1");
    door1.write(88);
    delay(door1CloseDelay);
    door1.write(90);
    door1pos = 0;
  }
  else if (door == 2) {
    door2.write(88);
    Serial.println("Closing door 2");
    delay(door2CloseDelay);
    door2.write(90);
    door2pos = 0;
  }
}

void toggleDoor(int door, int doorpos) {
  if (doorpos == 0) {
    openDoor(door);
  }
  else if (doorpos == 1) {
    closeDoor(door);
  }
}

int calibrate(int door, int dir) { //dir is 1 for open, 0 for close
  int startTime, endTime;
  if (door == 1) {
    if (dir == 1) {
      startTime = millis();
      door1.write(92);
      while (Serial.available() <= 0) {
        ;
      }
      door1.write(90);
      endTime = millis();
    }
    else if (dir == 0) {
      startTime = millis();
      door1.write(88);
      while (Serial.available() <= 0) {
        ;
      }
      door1.write(90);
      endTime = millis();
    }
  }
  else if (door == 2) {
    if (dir == 1) {
      startTime = millis();
      door2.write(92);
      while (Serial.available() <= 0) {
        ;
      }
      door2.write(90);
      endTime = millis();
    }
    else if (dir == 0) {
      startTime = millis();
      door2.write(88);
      while (Serial.available() <= 0) {
        ;
      }
      door2.write(90);
      endTime = millis();
    }
  }
  return endTime - startTime;
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
  Serial.println("");
}

