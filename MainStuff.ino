#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16

const int button = 12;
const int button_2 = 8;
const int laser = 2;
const int photo = A2;
const int pot = A0;
const int EnA = 11;
const int In1 = 10;
const int In2 = 4;
const int button_3 = 7;

using namespace std;

int ledList[16];

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50

float ledBrightness = 20;
bool button_2_pressed = 0;
int magentaNumber = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);


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
  pinMode(photo, INPUT);
  Serial.begin(9600);

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  lcd.init();
  lcd.backlight();
    
  lcd_both("Btn1 for count", "Btn2 to start");
  do{
    initial_player_indicator();
    if (digitalRead(button) == HIGH) {
      initial_player_indicator();
      if (magentaNumber == 7) {
        magentaNumber = 2;
        delay(250);
      } else {
        magentaNumber +=1;
        delay(250); 
      }
    }
  } while (digitalRead(button_2) == LOW);
  led_strip_shuffle();
  lcd_both("Press to shuffle", "Prss to dispense");
}

void loop() {
  
  if (digitalRead(button) == HIGH) {
    lcd_both("Shuffling the", "board");
    led_strip_shuffle();
    lcd_both("Press to shuffle", "Prss to dispense");
  }
    
  

  if (digitalRead(button_2) == HIGH) {
    lcd_both("Dispensing...","");
    run_motor();
    lcd_both("Press to shuffle", "Prss to dispense");
  }


  potentiometer();
  show_led_strip();
  
}

void show_led_strip() { //updates the led strip without shuffling the values found in the list
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

void led_strip_shuffle() { //shuffles the led strip lights and sends updates the list
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i+1, ledBrightness, ledBrightness, ledBrightness);
    int ledrand = rand() % 2; 
    if (ledrand == 0) {
      pixels.setPixelColor(i, ledBrightness, 0, 0);
    } else if (ledrand == 1) {
      pixels.setPixelColor(i, 0, 0, ledBrightness);
    }
    ledList[i] = ledrand;
    pixels.show();
    delay(DELAYVAL);
  }
  delay(250);
  for (int i = 0; i < magentaNumber; i++){
    int magentaReplace;
    do{ // this will reroll until it generates a number that isn't already magenta (2)
      magentaReplace = rand() % 16;
    } while(ledList[magentaReplace] == 2);
    
    pixels.setPixelColor(magentaReplace, ledBrightness, 0, ledBrightness);
    ledList[magentaReplace] = 2;
    pixels.show();
    delay(250);
  }
}

void laserRun() { //turns on the laser
  if (digitalRead(button_2) == HIGH) {
    digitalWrite(laser, HIGH);
  } else {
    digitalWrite(laser, LOW);
  }
}



void potentiometer() { //changes led strip brightness
  int brightness = analogRead(pot);

  brightness = map(brightness, 0, 1023, 5, 100);

  ledBrightness = brightness;
}


void initial_player_indicator(){ //used to show how many players will be playing
  for (int g = 0; g < 3; g++) {
    for (int i = 0; i < magentaNumber; i++) {
      pixels.setPixelColor(i, ledBrightness, ledBrightness, ledBrightness);
      pixels.show();
    }
    show_led_strip();
  }
}

void run_motor(){ //runs the motor unless the photoresistor is sending a value below 200
  do{
    int p = analogRead(photo);
    Serial.println(p);
    analogWrite(EnA, 50);
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(EnA, 50);
    digitalWrite(laser, HIGH);
    delay(250);
  } while (analogRead(photo) < 300);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(laser, LOW);
}

void lcd_both(String first, String second){ // print one string on each line of the LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(first);
  lcd.setCursor(0,1);
  lcd.print(second);
}


