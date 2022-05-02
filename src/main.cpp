#include <Arduino.h>
#include <LiquidCrystal.h>
#include "UI.h"
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Activate debug mode by defining DEBUG_MODE
// #define DEBUG_MODE

#define PIN_LDR A1
#define PIN_ERROR A2
#define PIN_LED 11
#define KEY_STATE A0

#define DELAY 100
#define HIGH_LUM 1000
#define LOW_LUM 150
#define TIMEOUT_TIME 10000

// Getting key values
enum Keys{btnRight = 0, btnUp, btnDown, btnLeft, btnSelect, btnNone};

// Identifies what keys are being pressed
// Right key press: LDR screen
// Left key press: LED screen
// Down key press: HOME screen
// Up key press: ALL screen
uint8_t getKey(uint16_t btnRead){
  if(btnRead >= 0 && btnRead < 80){
    return btnRight;
  }
  if(btnRead > 80 && btnRead < 225){
    return btnUp;
  }
  if(btnRead > 225 && btnRead < 395){
    return btnDown;
  }
  if(btnRead > 395 && btnRead < 625){
    return btnLeft;
  }
  if(btnRead > 625 && btnRead < 1000){
    return btnSelect;
  }
  if(btnRead > 1000 && btnRead <= 1023){
    return btnNone;
  }
}

// Prints all info on display
// Probably will go into UI.h
/*
void printInfo(uint8_t led_value, uint16_t adr_ldr){
  lcd.setCursor(7, 0);
  lcd.print("    "); //4 espaços
  lcd.setCursor(7, 0);
  lcd.print(analogRead(PIN_ERROR));
  lcd.setCursor(4, 1);
  lcd.print("    "); //4 espaços
  lcd.setCursor(4, 1);
  lcd.print(adr_ldr);
  lcd.setCursor(13, 1);
  lcd.print("   "); //3 espaços
  lcd.setCursor(13, 1);
  lcd.print(led_value);
}
*/

uint8_t ldr_to_led(uint16_t value, uint16_t lowlum, uint16_t highlum){
  if(value <= lowlum || value >= highlum)
  return 0;
  else{
    return map(value, lowlum, highlum, 50, 255);
  }
}

void setup(){
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_ERROR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, Character::leftArrow);
  lcd.createChar(1, Character::rightArrow);
  lcd.createChar(2, Character::upArrow);
  lcd.createChar(3, Character::downArrow);
  lcd.clear();
  lcd.home();
}

void loop(){
  // Was going to draw the hud every frame, but figured i'd draw it on a 100 ms refresh rate
  // UI::HUD::draw(lcd, led_value, adr_ldr);

  uint16_t adr_ldr = analogRead(PIN_LDR);
  uint8_t led_value = ldr_to_led(adr_ldr, LOW_LUM, HIGH_LUM);
  static uint32_t last_time = millis();
  analogWrite(PIN_LED, led_value);
  uint32_t actual_time = millis();

  // Reads what keys are pressed
  uint16_t btnRead = analogRead(KEY_STATE);
  uint8_t keyPressed = btnNone;

  // Reads if the key is pressed
  uint8_t btnState = digitalRead(KEY_STATE);
  uint8_t lastBtnState = 0;

  // Refreshes the display after 100ms
  if(actual_time - last_time >= DELAY){
    UI::HUD::draw(lcd, keyPressed, led_value, adr_ldr);
    // printInfo(led_value, adr_ldr);
    last_time = actual_time;
  }

  // Timeout logic
  static uint32_t lastTimeout = millis();
  uint32_t timeTimeout = millis();
  if(timeTimeout - lastTimeout >= TIMEOUT_TIME /*&& botão pressionado*/){
    lcd.noBacklight();
  }
  #ifdef DEBUG_MODE
  Serial.println(btnRead);
  // NONE = 1023
  // SELECT = 639
  // LEFT = 410
  // UP = 100
  // DOWN = 256
  // RIGHT = 0
  #endif

  // If key is pressed, gets what key is pressed
  if(btnState != lastBtnState){
    if(btnState == HIGH){
      keyPressed = getKey(btnRead);
    }
  }
  lastBtnState = btnState;
}