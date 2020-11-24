#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    2002212
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 1000

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


// LEDs
int brightness = 64;
bool increasing = true;
int maxBrightness = 255;
int minBrightness = 0;

// Audio
const int sampleWindow = 30; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {

  Serial.begin(115200);
  
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 255 );
    
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

}


void loop() {

  //modBrightness();

  //Serial.println(brightness);

  brightness = getBrightness();
  

  fillLEDs();
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  
}

void fillLEDs(){

  uint8_t colorIndex = 1;
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }

}

int getBrightness(){

  
  int volts = getAudioLevel();
  
  double level = map(volts, 0, 1024, 0, 255);
  //Serial.print("level:");Serial.println(level);
  return level;
  
}

int getAudioLevel(){

  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  // collect data for sampleWindow
  while (millis() - startMillis < sampleWindow){
    sample = analogRead(0);
    if (sample < 1024){  // toss out spurious readings
      if (sample > signalMax){
          signalMax = sample;  // save just the max levels
       }
       else if (sample < signalMin){
          signalMin = sample;  // save just the min levels
       }
    }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  
   //Serial.print("in level:");Serial.println(peakToPeak);
   return peakToPeak;
   //return volts;
}

// For brightness fade
void modBrightness(){

  if(brightness == maxBrightness){
    increasing = false;
  }
  if(brightness == minBrightness){
    increasing = true;
  }

  if(increasing){
    brightness++;
  }else brightness--;
  
}
