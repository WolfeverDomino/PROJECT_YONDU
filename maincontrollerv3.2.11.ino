/////////////////////////////////////////////
// Choosing your board
// Define your board, choose:
// "ATMEGA328" if using ATmega328 - Uno, Mega, Nano...
// "DEBUG" if you just want to debug the code in the serial monitor
#define ATMEGA328 1 //* put here the uC you are using, like in the lines above followed by "1", like "ATMEGA328 1", "DEBUG 1", etc.
// ALTER ^

/////////////////////////////////////////////
// LIBRARIES (DO NOT ALTER POSITIONS)
// === Library for the display === //
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// === Library for TimerOne === //
#include <TimerOne.h>

// === Library for RGB LED Strip === //
#include <FastLED.h>

// -- Defines the MIDI library -- //
// if using with ATmega328 - Uno, Mega, Nano...
#ifdef ATMEGA328
#include <MIDI.h> // by Francois Best
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

/////////////////////////////////////////////
//*********** CONFIGURE HERE **************//
/////////////////////////////////////////////

// MAIN PIN CONFIGURATION
#define P_SWITCH_PIN 22 //*Pin for Power Switch

#define CHANNELSWITCH1_PIN 24  //* pin of channel switcher connected to Arduino
#define CHANNELLED1_PIN 25 //* pin of channel led connected to Arduino
#define CHANNELSWITCH2_PIN 26   //* pin of channel switcher connected to Arduino
#define CHANNELLED2_PIN 27 //* pin of channel led connected to Arduino

#define LED_PIN 32  //* pin of LED
#define MAX_NUM_LEDS 16 //* Maximum number of LEDS on the strip
#define BUTTON_NUM_LEDS 16  //* Number of LEDS used for 4X4 Button Matrix
#define USING_NUM_LEDS 16 //* Number of LEDS being used in the entire build

#define rot_sw 11      //set the button on pin 3
#define rot_dt 10      //set the rot_dt on pin 4
#define rot_clk 9      //set the rot_clk on pin 5

const int N_BUTTONS = 16; //*  total numbers of buttons {53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38}
const int BUTTON_ARDUINO_PIN[N_BUTTONS] = {53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38}; //* pins of each button connected straight to the Arduino
const int N_POTS = 10; //* total numbers of pots (slide & rotary) eg. {A0, A1, A2, A3, A4, A5, A6, A7, A14, A15}
const int POT_ARDUINO_PIN[N_POTS] = {A0, A1, A2, A3, A4, A5, A6, A7, A14, A15}; //* pins of each pot connected straight to the Arduino

/////////////////////////////////////////////
//***DO NOT CONFIGURE BEYONG THIS POINT!***//
/////////////////////////////////////////////

// LCD CUSTOM CHARACTERS
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

//pitch & brightness level
uint8_t v0[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x1f};       //0
uint8_t v1[8] = {0x0,0x0,0x0,0x0,0x0,0x1f,0x1f};      //1-2
uint8_t v2[8] = {0x0,0x0,0x0,0x0,0x1f,0x1f,0x1f};     //3-5
uint8_t v3[8] = {0x0,0x0,0x0,0x1f,0x1f,0x1f,0x1f};    //6-8
uint8_t v4[8] = {0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f};   //9-11
uint8_t v5[8] = {0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};  //11-13
uint8_t v6[8] = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}; //14-15

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

/////////////////////////////////////////////
// STARTUP
int startup_oncer = 0;  //create a variable for actions to do once

/////////////////////////////////////////////
// POWER ROCKER SWITCH
int pswitchState = 0; // stores the button current value

/////////////////////////////////////////////
// CHANNEL SWITCHER
int channelswitchState1 = 0; // stores the button current value
int channelswitchState2 = 0; // stores the button current value
int P1_oncer = 0;
int P2_oncer = 0;
int P3_oncer = 0;

/////////////////////////////////////////////
// RGB LED STRIP
int bright_set = 128;  //  bright_set Level (Default = 68)(Max = 255)
int count_bright = 6;  //  count level of bright for bar (default = 4)

/////////////////////////////////////////////
// LCD & ROT
byte READ_BUTTON = 1;           // variable to read the button
byte READ_P_BUTTON = 1;         // variable to check the previous reading of the button
unsigned long tempor = 0;       // variable for counting time
int colorful = 0;               // variable for color palette chooser
int reference = 0;              // variable for encoder position reference
int directo = 0;                // variable to detect encoder directo
int rotato = 0;                 // variable to detect encoder rotation
byte scroll = 1;                //variable to control screen rot_switching in the menu
bool clear_lcd = 0;             //variable to clear all characters from lcd

///////////////////////////////////////////////////
// BUTTONS
int buttonCState[N_BUTTONS] = {};        // stores the button current value
int buttonPState[N_BUTTONS] = {};        // stores the button previous value

// Debounce
unsigned long lastDebounceTime[N_BUTTONS] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    //* the debounce time; increase if the output flickers

/////////////////////////////////////////////
// POTENTIOMETERS (INLUDING SLIDE POTENTIOMETERS)
int potCState[N_POTS] = {0}; // Current state of the pot
int potPState[N_POTS] = {0}; // Previous state of the pot
int potVar = 0; // Difference between the current and previous state of the pot

int midiCState[N_POTS] = {0}; // Current state of the midi value
int midiPState[N_POTS] = {0}; // Previous state of the midi value

const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[N_POTS] = {0}; // Previously stored time
unsigned long timer[N_POTS] = {0}; // Stores the time that has elapsed since the timer was reset

///////////////////////////////////////////////////
// COLORS
CRGB leds[USING_NUM_LEDS];

CRGB RED = CRGB( 128, 0, 0);
CRGB ORANGE = CRGB( 190, 15, 2);
CRGB ORAMBER = CRGB( 220, 10, 1); //Mix of orange and yellow
CRGB GOLD = CRGB( 255, 150, 0);
CRGB YELLOW = CRGB( 250, 200, 0);
CRGB STEEL = CRGB( 192, 192, 192);
CRGB BLACK = CRGB( 0, 0, 0);
CRGB GREEN = CRGB( 0, 255, 0);
CRGB NAVY = CRGB( 0, 0, 255);
CRGB LIGHTBLUE = CRGB( 50, 255, 255);
CRGB PINKY = CRGB( 50, 10, 10);
CRGB MAROON = CRGB( 100, 2, 2);
CRGB PURPLE = CRGB( 128, 0, 128);
CRGB QUARTZ = CRGB( 81, 65, 79);
CRGB REDPURPLE = CRGB( 149, 53, 83);
CRGB THISTLE = CRGB( 216, 191, 216);
CRGB VIOLET = CRGB( 143, 0, 255);
CRGB LAVENDER = CRGB( 80, 80, 255);
CRGB WHITE = CRGB( 255, 255, 255);
CRGB SMOKE = CRGB(105,105,105);
CRGB CHOC = CRGB(210,105,30);

/////////////////////////////////////////////
// MIDI
byte midiCh = 1; //* MIDI channel to be used
int startingkey = 60; //* startingkey to be used | Every set of keys is 12
byte cc = 1; //* Lowest MIDI CC to be used

// For Channel Switching
byte P1_SW = 2; //default channel for piano mode - 1
byte P2_SW = 2; //default channel for piano mode - 2
byte P3_SW = 2; //default channel for piano mode - 3

void setup(){
  // Baud Rate
  // use if using with ATmega328 (uno, mega, nano...)
  // 31250 for MIDI class compliant | 115200 for Hairless MIDI
  Serial.begin(115200);
  
  #ifdef DEBUG
  Serial.println("Debug mode");
  Serial.println();
  #endif

  // Power Rocker Switch
  pinMode(P_SWITCH_PIN, INPUT_PULLUP);

  // Channel Switcher
  pinMode(CHANNELSWITCH1_PIN, INPUT_PULLUP);
  pinMode(CHANNELLED1_PIN, OUTPUT);
  pinMode(CHANNELSWITCH2_PIN, INPUT_PULLUP);
  pinMode(CHANNELLED2_PIN, OUTPUT);
  
  // RGB LED Strip 
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, USING_NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();

  // Buttons
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
  }
  
  Timer1.initialize(1000);             //obs.: 1000000 is equal to 1 second. And, 1000 equals 1 millisecond
  Timer1.attachInterrupt(Timer);       //TimerOne is appended to "void Timer()"
  lcd.begin (16,2);                    //starts the 16x2 display
  lcd.noBacklight(); 
  lcd.createChar(8, v0);
  lcd.createChar(9, v1);
  lcd.createChar(10, v2);
  lcd.createChar(11, v3);
  lcd.createChar(12, v4);
  lcd.createChar(13, v5);
  lcd.createChar(14, v6);
  lcd.home();

  pinMode(rot_clk,INPUT);              //set rot_clk as input
  pinMode(rot_dt,INPUT);               //set rot_dt as input
  pinMode(rot_sw,INPUT_PULLUP);        //set rot_sw (button) as pullup input
  reference = digitalRead(rot_clk);    // read the rot_clk and store it in the reference variable
}
 
void loop(){
  //Channel Check
  channelswitchState1 = digitalRead(CHANNELSWITCH1_PIN);
  channelswitchState2 = digitalRead(CHANNELSWITCH2_PIN);

  if(startup_oncer==1){
    if (channelswitchState1==HIGH && channelswitchState2==LOW) {
      digitalWrite(CHANNELLED1_PIN, HIGH);
      digitalWrite(CHANNELLED2_PIN, LOW);
      chan1_on();
      }
    else if (channelswitchState2==HIGH && channelswitchState1==LOW) {
      digitalWrite(CHANNELLED1_PIN, LOW);
      digitalWrite(CHANNELLED2_PIN, HIGH);
      chan2_on();
    }
    else if (channelswitchState1==HIGH && channelswitchState2==HIGH) {
      digitalWrite(CHANNELLED1_PIN, HIGH);
      digitalWrite(CHANNELLED2_PIN, HIGH);
      chan3_on();
    }
    else{
      digitalWrite(CHANNELLED1_PIN, LOW);
      digitalWrite(CHANNELLED2_PIN, LOW);
      chan_off();
    }
  }
  
  //Power Check
  pswitchState = digitalRead(P_SWITCH_PIN);
  if(pswitchState==LOW){
    startup_on();
    //Actions on the buttons
    for (int i = 0; i < N_BUTTONS; i++) {
      buttons(i);
    }
    potentiometers();
    
    //Actions on the RGB LED Strip
    FastLED.show();
    FastLED.setBrightness(bright_set);
    
    //Actions on the LCD display
   if (scroll==1){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PROJ_YONDU  MENU"));     
     lcd.setCursor(0,1);                     
     lcd.printByte(0x7E); 
     lcd.print(F("PIANO MODE 1   "));      
   }
   else if (scroll==10){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PIANO MODE 1    "));      
     lcd.setCursor(5,1);
     lcd.print(P1_SW/2);                     
     lcd.print(F("                ")); 
   }   
   else if (scroll==2){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PROJ_YONDU  MENU"));    
     lcd.setCursor(0,1);                     
     lcd.printByte(0x7E); 
     lcd.print(F("PIANO MODE 2   "));    
   }
   else if (scroll==20){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PIANO MODE 2    "));      
     lcd.setCursor(5,1);
     lcd.print(P2_SW/2);                     
     lcd.print(F("                ")); 
   }
   else if (scroll==3){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PROJ_YONDU  MENU"));    
     lcd.setCursor(0,1);                     
     lcd.printByte(0x7E); 
     lcd.print(F("PIANO MODE 3   "));    
   }
   else if (scroll==30){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PIANO MODE 3    "));      
     lcd.setCursor(5,1);
     lcd.print(P3_SW/2);                     
     lcd.print(F("                ")); 
   }
   else if (scroll==4){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PROJ_YONDU  MENU"));    
     lcd.setCursor(0,1);                     
     lcd.printByte(0x7E); 
     lcd.print(F("STARTING KEY    "));    
   }
   else if (scroll==40){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("STARTING KEY    "));      
     lcd.setCursor(5,1);
     lcd.print(startingkey);                     
     lcd.print(F("                ")); 
   }
   else if (scroll==5){                        
     lcd.setCursor(0,0);                    
     lcd.print(F("PROJ_YONDU  MENU"));    
     lcd.setCursor(0,1);                    
     lcd.printByte(0x7E); 
     lcd.print(F("RGB COLORS     "));    
   }
   else if (scroll==50){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("RGB COLORS      "));      
     lcd.setCursor(5,1);                     
     lcd.print(colorful/2);   
     lcd.print(F("                "));    
   }
   else if (scroll==6){                        
     lcd.setCursor(0,0);                    
     lcd.print(F("PROJ_YONDU  MENU"));
     lcd.setCursor(0,1);                    
     lcd.printByte(0x7E); 
     lcd.print(F("RGB BRIGHTNESS "));    
   }
   else if (scroll==60){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("RGB BRIGHTNESS  "));                         
     brightcnt();   
   }
   else if (scroll==7){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("PROJ_YONDU  MENU"));   
     lcd.setCursor(0,1);                     
     lcd.printByte(0x7E); 
     lcd.print(F("RESET          "));    
   }
   else if (scroll==70){                            
     lcd.setCursor(0,0);                        
     lcd.print(F("RESETTING           "));      
     lcd.setCursor(0,1);                     
     lcd.print(F("."));
     delay(400);
     lcd.print(F("."));
     delay(400);
     lcd.print(F("."));
     tempor=0;   //time  
     colorful=0;
     startingkey = 60;
     P1_SW=2;
     P2_SW=2;
     P3_SW=2;
     bright_set = 68;
     count_bright = 4;
     presetcolor();
     delay(200);
     scroll=1;
   }
   if (clear_lcd==1){
     lcd.clear();
     clear_lcd=0;           
   }
  }
  else{
    startup_off();
  }
}

//////////////////////////////////////////////////////////////////
// STARTUP
void startup_on(){  //wake up Neo
  if(startup_oncer==0){  //upon startup
    if (channelswitchState1==HIGH) {
      bright_set = 34;
      count_bright = 2;
    }
    else{
      bright_set = 150;
      count_bright = 9;
    }
    scroll = 1;
    colorful = 0;
    startingkey = 60
    ;
    P1_SW=2;
    P2_SW=2;
    P3_SW=2;
    FastLED.setBrightness(bright_set);
    
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);                      
    lcd.print(F(" PROJECT  YONDU "));   
    lcd.setCursor(0,1);                     
    lcd.print(F(" BY BERSERK.INC ")); 
    for (int i = 0; i < USING_NUM_LEDS; i++) {
      leds[i] = RED;
      FastLED.show();
      delay(40);
    }
    lcd.setCursor(0,0);
    lcd.print(F(" PROD. BY RUBEN "));   
    lcd.setCursor(0,1);                     
    lcd.print(F(" ERFAN  RIDHWAN ")); 
    for (int i = 0; i < USING_NUM_LEDS; i++) {
      leds[i] = BLACK;
      FastLED.show();
      delay(40);
    }
    lcd.clear();
    delay(10);
    startup_oncer=1;
  }
}
void startup_off(){
  if (startup_oncer==1){
    lcd.clear();
    lcd.setCursor(0,0);                        
    lcd.print(F("    GOODBYE     "));
    for (int i = 0; i < USING_NUM_LEDS; i++) {
      leds[i] = RED;
    }
    for(int i = 0; i > 120; i++){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(5);
    }
    for(int i = 120; i > 0; i--){
      FastLED.setBrightness(i);
      FastLED.show();
      delay(5);
    }
    FastLED.clear();
    FastLED.setBrightness(0);
    FastLED.show();
    delay(500);
    lcd.clear();
    lcd.noBacklight();
    startup_oncer=0;
  }
}

//////////////////////////////////////////////////////////////////////////
// CHANNEL STATE - PIANO
void chan1_on(){  //wake up Neo
  if(P1_oncer==0){  //upon startup
    bright_set = 102;
    count_bright = 6;
    for (int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = CHOC;
      FastLED.show();
      FastLED.setBrightness(bright_set);
    }
    P1_oncer=1;
    P2_oncer=0;
    P3_oncer=0;
  }
}
void chan2_on(){  //wake up Neo
  if(P2_oncer==0){  //upon startup
    bright_set = 111;
    count_bright = 7;
    for (int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = ORAMBER;
      FastLED.show();
      FastLED.setBrightness(bright_set);
    }
    P1_oncer=0;
    P2_oncer=1;
    P3_oncer=0;
  }
}
void chan3_on(){
  if(P3_oncer==0){
    bright_set = 221;
    count_bright = 12;
    for (int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = PINKY;
      FastLED.show();
      FastLED.setBrightness(bright_set);
    }
    P1_oncer=0;
    P2_oncer=0;
    P3_oncer=1;
  }
}
void chan_off(){
  if(P1_oncer==1 || P2_oncer==1 || P3_oncer==1){
    FastLED.clear();
    bright_set = 68;
    count_bright = 4;
    presetcolor();
    P1_oncer=0;
    P2_oncer=0;
    P3_oncer=0;
  }
}

//////////////////////////////////////////////////////////////////////////
// BUTTONS
void buttons(int i){
    buttonCState[i] = digitalRead(BUTTON_ARDUINO_PIN[i]);  // read pins from arduino
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();
        if (buttonCState[i] == LOW) {
          rgb_on(i);
          // Sends the MIDI startingkey ON accordingly to the chosen board
#ifdef ATMEGA328
// use if using with ATmega328 (uno, mega, nano...)
if (channelswitchState1==LOW &&channelswitchState2==LOW) {
  MIDI.sendNoteOn(startingkey, 127, midiCh+i); // startingkey, velocity, channel
}
else if (channelswitchState1==HIGH && channelswitchState2==LOW) {
  MIDI.sendNoteOn(startingkey+i, 127, (P1_SW/2)); // C4, velocity, channel
}
else if (channelswitchState1==LOW && channelswitchState2==HIGH) {
  MIDI.sendNoteOn(startingkey+i , 127, (P2_SW/2)); // startingkey, velocity, channel
}
else if (channelswitchState1==HIGH && channelswitchState2==HIGH) {
  MIDI.sendNoteOn(startingkey+i , 127, (P3_SW/2)); // startingkey, velocity, channel
}

#elif DEBUG
if (channelswitchState1==LOW) {
  Serial.print("Butt: ");
  Serial.print(i);
  Serial.print(" | Note: ");
  Serial.print((startingkey/2));
  Serial.print(" | MidiCh: ");
  Serial.print((P1_SW/2)+i);
  Serial.println(": button on");
}
else{
  Serial.print("Butt: ");
  Serial.print(i);
  Serial.print(" | Note: ");
  Serial.print((startingkey/2)+i);
  Serial.print(" | MidiCh: ");
  Serial.print(midiCh);
  Serial.println(": button on");
}
#endif
        }
        else {
          rgb_off(i);
          // Sends the MIDI startingkey OFF accordingly to the chosen board
#ifdef ATMEGA328
// use if using with ATmega328 (uno, mega, nano...)
if (channelswitchState1==LOW &&channelswitchState2==LOW) {
  MIDI.sendNoteOn(startingkey, 0, midiCh + i); // startingkey, velocity, channel
}
else if (channelswitchState1==HIGH && channelswitchState2==LOW) {
  MIDI.sendNoteOn(startingkey+i, 0, (P1_SW/2)); // startingkey, velocity, channel
}
else if (channelswitchState1==LOW && channelswitchState2==HIGH){
  MIDI.sendNoteOn(startingkey+i , 0, (P2_SW/2)); // startingkey, velocity, channel
}
else if (channelswitchState1==HIGH && channelswitchState2==HIGH){
  MIDI.sendNoteOn(startingkey+i , 0, (P3_SW/2)); // startingkey, velocity, channel
}
#elif DEBUG
Serial.print("Butt: ");
Serial.print(i);
Serial.println(": button off");
#endif
      }
      buttonPState[i] = buttonCState[i];
    }
  }
}

////////////////////////////////////////////////////////////////////
// POTENTIOMETERS
void potentiometers() {
  for (int i = 0; i < N_POTS; i++) { // Loops through all the potentiometers
    potCState[i] = analogRead(POT_ARDUINO_PIN[i]); // reads the pins from arduino
    midiCState[i] = map(potCState[i], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi
    potVar = abs(potCState[i] - potPState[i]); // Calculates the absolute value between the difference between the current and previous state of the pot
    if (potVar > varThreshold) { // Opens the gate if the potentiometer variation is greater than the threshold
      PTime[i] = millis(); // Stores the previous time
    }
    timer[i] = millis() - PTime[i]; // Resets the timer 11000 - 11000 = 0ms
    if (timer[i] < TIMEOUT) { // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
      potMoving = true;
    }
    else {
      potMoving = false;
    }
    if (potMoving == true) { // If the potentiometer is still moving, send the change control
      if (midiPState[i] != midiCState[i]) {
        // Sends the MIDI CC accordingly to the chosen board
#ifdef ATMEGA328
// use if using with ATmega328 (uno, mega, nano...)
MIDI.sendControlChange(cc + i, midiCState[i], midiCh); // cc number, cc value, midi channel

#elif DEBUG
Serial.print("Pot: ");
Serial.print(i);
Serial.print(" ");
Serial.print(midiCh);
Serial.print(" ");
Serial.println(midiCState[i]);
//Serial.print("  ");
#endif

        potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
        midiPState[i] = midiCState[i];
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
// RGB LED STRIP CONFIG PRESSED/RELEASED/TURNED
void rgb_on(int i) {
  if (channelswitchState1==HIGH && channelswitchState2==LOW){
    leds[i] = BLACK;
  }
  else if (channelswitchState1==LOW && channelswitchState2==HIGH){
    leds[i] = MAROON;
  }
  else if (channelswitchState1==HIGH && channelswitchState2==HIGH){
    leds[i] = PURPLE;
  }
  else{
    switch (colorful/2) {
      case 1:
            leds[i] = GOLD; //starting color palette
        break;
      case 2:
            leds[i] = ORAMBER;
        break;
      case 3:
            leds[i] = LIGHTBLUE;
        break;
      case 4:
            leds[i] = MAROON;
        break;
      case 5:
            leds[i] = REDPURPLE;
        break;
      case 6:
            leds[i] = THISTLE;
        break;
      case 7:
            leds[i] = BLACK;
        break;
      default:
            leds[i] = STEEL;
    }
  }
}
void rgb_off(int i) {
  if (channelswitchState1==HIGH && channelswitchState2==LOW){
    leds[i] = CHOC;
  }
  else if (channelswitchState1==LOW && channelswitchState2==HIGH){
    leds[i] = ORAMBER;
  }
  else if (channelswitchState1==HIGH && channelswitchState2==HIGH){
    leds[i] = PINKY;
  }
  else{
    switch (colorful/2) {
      case 1:
            leds[i] = RED;  //starting color palette
        break;
      case 2:
            leds[i] = GREEN;
        break;
      case 3:
            leds[i] = NAVY;
        break;
      case 4:
            leds[i] = ORAMBER;
        break;
      case 5:
            leds[i] = VIOLET;
        break;
      case 6:
            leds[i] = LAVENDER;
        break;
      case 7:
            leds[i] = CHOC;
        break;
      default:
            leds[i] = BLACK;
    }
  }
}

////////////////////////////////////////////////////////////
// PRESET COLOR PALLETE
void presetcolor() {
  if (channelswitchState1==HIGH && channelswitchState2==LOW){
    for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = WHITE;
    }
  }
  else if (channelswitchState1==LOW && channelswitchState2==HIGH){
    for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = ORAMBER;
    }
  }
  else if (channelswitchState1==HIGH && channelswitchState2==HIGH){
    for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
      leds[i] = PINKY;
    }
  }
  else{
    switch (colorful/2) { //starting color palette
      case 1: // IRON MAN
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = RED;
        }
        break;
      case 2: // CHRISTMAS
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = GREEN;
        }
        break;
      case 3: // SEA
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = NAVY;
        }
        break;
      case 4: // CHERRY
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = ORAMBER;
        }
        break;
      case 5: // 
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = VIOLET;
        }
        break;
      case 6: // 
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = LAVENDER;
        }
        break;
      case 7: // 
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = CHOC;
        }
        break;
      default:  // CONCERT
        // turn all the LEDs off:
        for( int i = 0; i < BUTTON_NUM_LEDS; ++i) {
          leds[i] = BLACK;
      }
    }
  }
}

/////////////////////////////////////////////
// BRIGHT LEVEL FOR LCD
void brightcnt(){
  lcd.setCursor(0,1);  
  for(int i = 0; i < count_bright; i++){
    if(i==0){
      lcd.printByte(8);
    }
    else if(i<=2){
      lcd.printByte(9);
    }
    else if(i<=5){
      lcd.printByte(10);
    }
    else if(i<=8){
      lcd.printByte(11);
    }
    else if(i<=11){
      lcd.printByte(12);
    }
    else if(i<=14){
      lcd.printByte(13);
    }
    else if(i==15){
      lcd.printByte(14);
    }
  }
  lcd.print(F("                ")); 
}

///////////////////////////////////////////////////////
// TIMER
void Timer (){      
// === KY 040 Control === //

  //Rotation
   rotato = digitalRead(rot_clk);          // read the rot_clk and store it in the rotation variable
   if (rotato != reference){               //if the rotation is different from the reference, do...
   if (digitalRead(rot_dt) != rotato) {    //if the digital reading of the rot_dt is different from the rotation, do...
     directo--;                            // subtract 1 from address value
    if (scroll==10){
      if((P1_SW)>=32){P1_SW=32;}  //16x2=32
      else{P1_SW++;}
    }
    if (scroll==20){
      if((P2_SW)>=32){P2_SW=32;}  //16x2=32
      else{P2_SW++;}
    }
    if (scroll==30){
      if((P3_SW)>=32){P3_SW=32;}  //16x2=32
      else{P3_SW++;}
    }
    if (scroll==40){
      if((startingkey/2)>=132){startingkey=132*2;}
      else{startingkey++;}
    }
    if (scroll==50){
      if((colorful/2)>=7){colorful=14;}
      else{colorful++;}
      presetcolor();
    }
    if (scroll==60){
      if(bright_set>=255){bright_set=255;}
      else{bright_set=bright_set+17;} //set 17 for perfect 15 turns without decimal
      if(count_bright>=15){count_bright=15;}  //main
      else{count_bright++;}
    }
   }
   else {                                   
     directo++;
     if (scroll==10){
      if(P1_SW<=2){P1_SW=2;}
      else{P1_SW--;}
     }
     if (scroll==20){
      if(P2_SW<=2){P2_SW=2;}
      else{P2_SW--;}
     }
     if (scroll==30){
      if(P3_SW<=2){P3_SW=2;}
      else{P3_SW--;}
     }
     if (scroll==40){
      if(startingkey<=0){startingkey=0;}
      else{startingkey--;}
     }
     if (scroll==50){
      if(colorful<=0){colorful=0;}
      else{colorful--;}
      presetcolor();
     }
     if (scroll==60){
      if(bright_set<=0){bright_set=0;}
      else{bright_set=bright_set-17;}
      if(count_bright<=0){count_bright=0;}
      else{count_bright--;}
     }
  }
   #ifdef ATMEGA328
// use if using with ATmega328 (uno, mega, nano...)

#elif DEBUG

#endif
   
   //Scroll for main screen
   if (directo == 2) {
     if (scroll==1)scroll=7;
     else if (scroll==2)scroll=1;
     else if (scroll==3)scroll=2;  
     else if (scroll==4)scroll=3;  
     else if (scroll==5)scroll=4;
     else if (scroll==6)scroll=5;
     else if (scroll==7)scroll=6;
     directo = 0;
     }
   if (directo == -2) {  
     if (scroll==6)scroll=7;
     else if (scroll==7)scroll=1;
     else if (scroll==1)scroll=2;
     else if (scroll==2)scroll=3;
     else if (scroll==3)scroll=4;
     else if (scroll==4)scroll=5;
     else if (scroll==5)scroll=6;
     directo = 0;   
     }
  } 
  reference = rotato;

  // === rot_sw Button Activation === //
  READ_BUTTON = digitalRead(rot_sw);
  if (READ_BUTTON == 0 && READ_P_BUTTON == 1) {    
      clear_lcd=1; 
           if (scroll==1)scroll=10;
      else if (scroll==2)scroll=20;
      else if (scroll==3)scroll=30;
      else if (scroll==4)scroll=40;
      else if (scroll==5)scroll=50;
      else if (scroll==6)scroll=60;
      else if (scroll==7)scroll=70;
      else if (scroll==10)scroll=1;
      else if (scroll==20)scroll=2;
      else if (scroll==30)scroll=3;   
      else if (scroll==40)scroll=4;  
      else if (scroll==50)scroll=5;
      else if (scroll==60)scroll=6;
      else if (scroll==70)scroll=7;
      delay(30);
      }                         
      READ_P_BUTTON = READ_BUTTON;
  tempor++;
}
