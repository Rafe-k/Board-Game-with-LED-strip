// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
//#include <ArduinoSTL.h>
//using namespace std;
//#include <Arduino_Threads.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6  // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16  // Popular NeoPixel ring size

const int button = 12;
const int button_2 = 8;
const int laser = 2;
const int photo = A5;
const int pot = A0;
const int EnA = 11;
const int In1 = 3;
const int In2 = A2;
const int button_3 = 7;

using namespace std;

int ledList[16];
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50  // Time (in milliseconds) to pause between pixels

float ledBrightness = 20;
bool button_2_pressed = 0;
int magentaNumber = 4; //actual number is one lower
int button_3_count = 4;



void setup() {
  
  pinMode(laser, OUTPUT);
  pinMode(photo, INPUT);
  pinMode(button, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(pot, INPUT);
  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  Serial.begin(9600);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  // #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //   clock_prescale_set(clock_div_1);
  // #endif
  // END of Trinket-specific code.

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  led_strip_shuffle();
}

void loop() {
  
  if (digitalRead(button) == HIGH) {
    led_strip_shuffle();
  }

  laserRun();
  if (digitalRead(button_2 == HIGH)) {
    //ledBrightWave();
  }
    
  float p = analogRead(photo);
  Serial.println(p);

  //button_2_press();
  potentiometer();
  show_led_strip();

  if (digitalRead(button_3) == HIGH) {
    button_3_press();
  }
  
  
}

void show_led_strip() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledList[i] == 0) {
      pixels.setPixelColor(i, ledBrightness, 0, 0);
    } else if (ledList[i] == 1) {
      pixels.setPixelColor(i, 0, 0, ledBrightness);
    } else if (ledList[i] == 2) {
      pixels.setPixelColor(i, ledBrightness, 0, ledBrightness);
    }
  }
  pixels.show();
}

void led_strip_shuffle() {
  pixels.clear();  // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i+1, ledBrightness, ledBrightness, ledBrightness);
    int ledrand = rand() % 2; 
    if (ledrand == 0) {
      pixels.setPixelColor(i, ledBrightness, 0, 0);
    } else if (ledrand == 1) {
      pixels.setPixelColor(i, 0, 0, ledBrightness);
    }
    ledList[i] = ledrand;
    pixels.show();  // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);
  }
  delay(250);
  for (int i = 0; i < magentaNumber; i++){
    int magentaReplace = rand() % 16;
    pixels.setPixelColor(magentaReplace, ledBrightness, 0, ledBrightness);
    ledList[magentaReplace] = 2;
    pixels.show();
    delay(250);
  }
}

void laserRun() {
  if (digitalRead(button_2) == HIGH) {
    digitalWrite(laser, HIGH);
  } else {
    digitalWrite(laser, LOW);
  }
}




void potentiometer() {
  int brightness = analogRead(pot);

  brightness = map(brightness, 0, 1023, 5, 100);

  ledBrightness = brightness;
}

/*void ledBrightWave() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledList[i] == 0) {
      //pixels.setPixelColor(i, ledBrightness, 0, 0);
      pixels.setPixelColor(i, ledBrightness, ledBrightness, ledBrightness);
    } else if (ledList[i] == 1) {
      //pixels.setPixelColor(i, 0, 0, ledBrightness);
      pixels.setPixelColor(i, ledBrightness, ledBrightness, ledBrightness);
    } else if (ledList[i] == 2) {
      //pixels.setPixelColor(i, ledBrightness, 0, ledBrightness);
      pixels.setPixelColor(i, ledBrightness, ledBrightness, ledBrightness);
    }
    delay(100);
  }
}*/

void button_3_press() {

  for (int g = 0; g < 3; g++) {
    for (int i = 0; i < button_3_count; i++) {
      pixels.setPixelColor(i, ledBrightness, ledBrightness, ledBrightness);
    }
    delay(1000);
    show_led_strip();
    delay(1000);
  }
}
