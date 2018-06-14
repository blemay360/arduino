#include <Servo.h>

#define led_pin 10
#define lower_servo_pin 6 //The circular motion one
#define upper_servo_pin 8 //The 180 degrees one
#define button_pin 9

Servo upperServo;
Servo lowerServo;

int pos = 0;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  upperServo.attach(upper_servo_pin);
  lowerServo.attach(lower_servo_pin);
  lowerServo.write(90);
}

void loop() {  
  if (digitalRead(button_pin) == HIGH) {
    brew();
  }
  lowerServo.write(90);
}

void brew() {
  bring_down();
  circle();
  bring_up();
  for (int i; i < 2; i++) {
    led_blink();
  }
}

void bring_down() {
  upperServo.write(0);
  delay(2000);
}

void bring_up() {
  upperServo.write(160);
  delay(1000);
}

void led_blink() {
  for (int i = 0; i < 5; i+=1) {
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
  }
}

void circle() {
  for(int i; i < 10; i++) {
    lowerServo.write(180);
    delay(1000);
  }
  lowerServo.write(90);
  delay(1000);
}

