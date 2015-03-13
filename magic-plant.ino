#include <FastLED.h>

#define LED_PIN     6
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    9
#define STARTUP_DELAY 1000
#define MAX_DELAY_SPEED  100

int potHuePin = A4;
float potHueVal = 0;
int hue = 0;

int potBrightnessPin = A2;
float potBrightnessVal = 0;
int brightness = 240;

int potDelayPin = A6;
float potDelayVal = 0;
int delayspeed = MAX_DELAY_SPEED;

unsigned long lastUpdateTime = 0;
unsigned long currentStep = 0;

CRGB leds[NUM_LEDS];

void setup() {
  delay(STARTUP_DELAY);
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  lastUpdateTime = millis();
}

void loop() {
  // between 0.0 and 1.0, takes into account that we are on the 3.3V rail
  potHueVal = (analogRead(potHuePin) * (1 / (1023.0 * (3.3 / 5.0))));
  hue = (int)floor(potHueVal * 255);
  
  if (hue >= 255) {
    hue = 0;
  }

//  potBrightnessVal = (analogRead(potBrightnessPin) * (1 / (1023.0 * (3.3 / 5.0))));
//  brightness = (int)floor(potBrightnessVal * 255);
//  
//  if (brightness >= 240) {
//    brightness = 240;
//  }  

  potDelayVal = (analogRead(potDelayPin) * (1 / (1023.0 * (3.3 / 5.0))));
//  delayspeed = (int)floor(potDelayVal * 255);

  if (millis() - lastUpdateTime > delayspeed) {
    lastUpdateTime = millis();
    currentStep++;
  }
  
  chase(currentStep);
  FastLED.show();
}

// a chase through colors
void chase(unsigned long currentStep) {
  int hueOffset = 70; // The value to add to Hue to build the spectrum
  int hueDiff = hueOffset / NUM_LEDS;
  int valueHolder [NUM_LEDS];
  int currentIndex = 0;
  
  for(int j = 0; j < NUM_LEDS; j++) {
    valueHolder[j] = hue + (j * hueDiff) % 255;
  }
  
  for(int i = 0; i < NUM_LEDS; i++) {
    int s = (i + currentStep) % NUM_LEDS;
    leds[i] = CHSV(valueHolder[s], 255, brightness);
  }
}
