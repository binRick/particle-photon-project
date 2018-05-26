#include "Particle.h"
#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);
#define PIXEL_PIN D4
#define PIXEL_COUNT 64
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

int ledBlue   = A7;
int ledYellow = A6;
//int ledGreen  = A5;
int toggleBlue(String command);


void setup() {
	strip.begin();
	strip.show();
	pinMode(ledBlue, OUTPUT);
	pinMode(ledYellow, OUTPUT);
//	pinMode(ledGreen, OUTPUT);
	digitalWrite(ledBlue, LOW);
	digitalWrite(ledYellow, LOW);
//	digitalWrite(ledGreen, LOW);
	Particle.function("toggleBlue", toggleBlue);

}

void loop() {
	rainbow(20);
//	digitalWrite(ledYellow, HIGH);
//	digitalWrite(ledGreen, HIGH);
	delay(1000);
//	digitalWrite(ledYellow, LOW);
//	digitalWrite(ledGreen, LOW);
	delay(1000);
}

int toggleBlue(String command){
  if(command == "on"){
	digitalWrite(ledBlue, HIGH);
	return 100;
  }else if(command == "off"){
	digitalWrite(ledBlue, LOW);
	return 10;
  }else
	return -1;
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
