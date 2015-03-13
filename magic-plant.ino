#include <FastLED.h>

#define LED_PIN     6
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    9
#define STARTUP_DELAY 1000
#define MAX_DELAY_SPEED  100

int potPin = A4;
int potVal = 0;
int hue = 0;
int brightness = 250;
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
  potVal = (analogRead(potPin) * (1 / (1023.0 * (3.3 / 5.0))));
  hue = (int)floor(potVal * 255);
  
  if (hue >= 255) {
    hue = 0;
  }
  
  //delayspeed = (MAX_DELAY_SPEED / 256) * (int)floor(potVal);
  
  if (brightness >= 255) {
    brightness = 0;
  }

  if (millis() - lastUpdateTime > delayspeed) {
    lastUpdateTime = millis();
    currentStep++;
  }
  
  chase(currentStep);
  FastLED.show();
}

// a chase through colors
void chase(unsigned long currentStep) {
  int brightnessDiff = 16;
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
