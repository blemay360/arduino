
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LEDPIN 6
#define BUTTONPIN 5

#define NUM_LEDS 30

#define BRIGHTNESS 100

#define orange strip.Color(200, 46, 0, 100)

int buttonState = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDPIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  pinMode(BUTTONPIN, INPUT);
}

void loop() {
  buttonState = readbutton(buttonState);
  if (buttonState == 0) {
    colorWipe(strip.Color(0,0,0,0));
  }
  else {
    colorWipe(orange);
  }
}

int readbutton(int buttonState) {
  if (digitalRead(BUTTONPIN) == HIGH) {
    if (buttonState == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

void colorWipe(uint32_t color) {
  for(int i = 0; i <= strip.numPixels() / 2; i++) {
      strip.setPixelColor(i, color);
      strip.setPixelColor(strip.numPixels() - i, color);
      strip.show();
      delay(50);
  }
}
