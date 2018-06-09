#include "Particle.h"
#include "Adafruit_GFX.h"
#include "neomatrix.h"
#include "neopixel.h"
#include "HMC5883L_Simple.h"
#include "Wire.h"

#define PIXEL_PIN D4
#define PIXEL_COUNT 64
#define PIXEL_TYPE WS2812B

HMC5883L_Simple Compass;

bool useStrip = true;
bool useMatrix = false;

int counter = 0;

 Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIXEL_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  PIXEL_TYPE);

 const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

 Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


int togglePixels(String command);
int getDirection(String command);

void colorAll(uint32_t c, uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);

uint32_t brightness = 100;

SYSTEM_MODE(AUTOMATIC);

int x = matrix.width();
  int pass = 0;
void setup() {
  Serial.begin(9600);

  // Compass
  Wire.begin();
  Compass.SetDeclination(-6, 25, 'W'); //http://www.magnetic-declination.com/
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);

  // neoPixel 8x8
  Particle.function("togglePixels", togglePixels);
  Particle.function("getDirection", getDirection);

 if(useMatrix){
  // neoPixelMatrix 8x8
  matrix.setTextWrap(false);
  matrix.setBrightness(30);
  matrix.setTextColor(matrix.Color(80,255,0));

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Howdy"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);


 }
  if(useStrip){
    strip.begin();
    strip.show();
  }

}


int getDirection(String command){
   float heading = Compass.GetHeadingDegrees();
   return (int) heading * 100000;
}
int togglePixels(String command){

if(useStrip){
  if(command == "rainbow"){
    rainbow(20);
  }else if(command == "rainbowCycle"){
    rainbowCycle(20);
  }else if(command == "red"){
    colorAll(strip.Color(255, 0, 0), brightness);
  }else if(command == "green"){
    colorAll(strip.Color(0, 255, 0), brightness);
  }else if(command == "blue"){
    colorAll(strip.Color(0, 0, 255), brightness);
  }else if(command == "redWipe"){
    colorWipe(strip.Color(255, 0, 0), 10);
  }else if(command == "greenWipe"){
    colorWipe(strip.Color(0, 255, 0), 10);
  }else if(command == "blueWipe"){
    colorWipe(strip.Color(0, 0, 255), 10);
  }else if(command == "off"){
    colorAll(strip.Color(0, 0, 0), 0);
  }
  return -1;
}
  return 999;
}

void loop() {
/*
 if(useMatrix){
  int x = matrix.width();
  int pass = 0;
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Howdy"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
 }
*/
}
void colorAll(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
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

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
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
