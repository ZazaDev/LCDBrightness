// Nome: Allan Duarte Ehlert
// Turma: TAI3M
// Controle de LED via LDR

#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include "UI.h"

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

// ╔═════════════════════════════════════════╗
// ║               INFO TABLE                ║
// ╠═════════════════════════════════════════╣
// ║ Key press menu interactions             ║
// ║ Right key press: LDR screen             ║
// ║ Left key press: LED screen              ║
// ║ Down key press: HOME screen             ║
// ║ Up key press: ALL screen                ║
// ╠═════════════════════════════════════════╣
// ║ ADC values on each button press         ║
// ║ NONE = 1023                             ║
// ║ SELECT = 639                            ║
// ║ LEFT = 410                              ║
// ║ UP = 100                                ║
// ║ DOWN = 256                              ║
// ║ RIGHT = 0                               ║
// ╚═════════════════════════════════════════╝

// Identifies what keys are being pressed
uint8_t getKey(uint16_t btnRead) {
  if (btnRead >= 0 && btnRead < 80) {
    return btnRight;
  }
  if (btnRead > 80 && btnRead < 225) {
    return btnUp;
  }
  if (btnRead > 225 && btnRead < 395) {
    return btnDown;
  }
  if (btnRead > 395 && btnRead < 625) {
    return btnLeft;
  }
  if (btnRead > 625 && btnRead < 1000) {
    return btnSelect;
  }
  return btnNone;
}

uint8_t ldr_to_led(uint16_t value, uint16_t lowlum, uint16_t highlum) {
  if (value <= lowlum || value >= highlum)
    return 0;
  else {
    return map(value, lowlum, highlum, 50, 255);
  }
}

void setup() {
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
  lcd.setBacklightPin(10, POSITIVE);
  lcd.backlight();
  // Initializes the HUD by drawing the home page
  UI::HUD::draw(lcd, btnDown, 0, 0);
}

void loop() {
  // Was going to draw the hud every frame, but figured i'd draw it on a 100 ms
  // refresh rate UI::HUD::draw(lcd, keyPressed, led_value, adr_ldr);
  static bool isTimeout = false;
  static bool ligar_led = true;
  uint16_t adr_ldr = analogRead(PIN_LDR);
  uint16_t btnRead = analogRead(KEY_STATE);
  uint8_t led_value = ldr_to_led(adr_ldr, LOW_LUM, HIGH_LUM);
  static uint32_t last_time = 0;

  if (ligar_led)
    analogWrite(PIN_LED, led_value);
  else {
    analogWrite(PIN_LED, 0);
    isTimeout = true;
  }
  uint32_t actual_time = millis();

  // Reads what keys are pressed
  static uint8_t keyPressed = btnDown;

  // Refreshes the display after 100ms
  if (actual_time - last_time >= DELAY) {
    UI::HUD::draw(lcd, keyPressed, led_value, adr_ldr);
#ifdef DEBUG_MODE
    Serial.println("HUD refreshed.");
#endif
    last_time = actual_time;
  }

  // Timeout logic
  static uint32_t lastTimeout = 0;
  uint32_t timeTimeout = millis();
  if (timeTimeout - lastTimeout >= TIMEOUT_TIME) {
    isTimeout = true;
    lastTimeout = timeTimeout;
  }
  if (isTimeout) {
    lcd.noBacklight();
  }
  if (!isTimeout) {
    lcd.backlight();
  }

  // Prints the information on the serial monitor

  // Reads if the key is pressed
  uint16_t btnState = analogRead(KEY_STATE);
  static uint16_t lastBtnState = 1;
  static uint32_t last_time_btn = millis();
  uint32_t time_btn = millis();

  // If key is pressed, gets what key is pressed
  if (abs(btnState - lastBtnState) > 25 && (time_btn - last_time_btn) >= 50) {
    if (btnState < 1000) {
      keyPressed = getKey(btnRead);
      Serial.print("Button pressed: ");
      Serial.println(keyPressed);
      if (keyPressed == btnSelect)
        ligar_led = !ligar_led;
      UI::HUD::draw(lcd, keyPressed, led_value, adr_ldr);
      lastTimeout = millis();
      isTimeout = false;
    }
    last_time_btn = time_btn;
  }
  lastBtnState = btnState;
}