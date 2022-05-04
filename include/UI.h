// Include guard, #pragma once doesn't seem to be used in Arduino code because
// apparently the Arduino IDE copies some files around the place when building.
#ifndef UI_H
#define UI_H

#define PIN_ERROR A2

#include <Arduino.h>
#include <LiquidCrystal.h>

enum Keys { btnRight = 0, btnUp, btnDown, btnLeft, btnSelect, btnNone };

// Creating custom characters for the UI
namespace Character {
byte leftArrow[8] = {B00000, B00010, B00110, B01110,
                     B01110, B00110, B00010, B00000};
byte rightArrow[8] = {B00000, B01000, B01100, B01110,
                      B01110, B01100, B01000, B00000};
byte upArrow[8] = {B00000, B00000, B00100, B01110,
                   B11111, B11111, B00000, B00000};
byte downArrow[8] = {B00000, B00000, B11111, B11111,
                     B01110, B00100, B00000, B00000};
} // namespace Character

namespace UI {
class HUD {
public:
  // Drawing homepage
  // BYTE0 Left key press: LED screen
  // BYTE1 Right key press: LDR screen
  // BYTE2 Up key press: ALL screen
  // BYTE3 Down key press: HOME screen
  static void draw(LiquidCrystal &lcd, uint8_t keyNum, uint8_t led_value,
                   uint16_t adr_ldr) {
    static bool refresh = false;
    static uint8_t lastKeyNum = btnDown;

    if (lastKeyNum != keyNum) {
      refresh = false;
      lastKeyNum = keyNum;
    }

    switch (keyNum) {
    case btnRight:
      if (!refresh) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("LDR: ");
        lcd.setCursor(4, 0);
        lcd.print(adr_ldr);
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print("LED ");
        lcd.setCursor(6, 1);
        lcd.write(byte(2));
        lcd.print("ALL  ");
        lcd.setCursor(11, 1);
        lcd.print("HOME");
        lcd.write(byte(3));

        refresh = true;
      } else {
        lcd.setCursor(4, 0);
        lcd.print("    ");
        lcd.setCursor(4, 0);
        lcd.print(adr_ldr);
      }
      break;

    case btnUp:
      if (!refresh) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ERRO: ");
        lcd.setCursor(5, 0);
        lcd.print(analogRead(PIN_ERROR));
        lcd.setCursor(0, 1);
        lcd.print("LDR: ");
        lcd.setCursor(4, 1);
        lcd.print(adr_ldr);
        lcd.setCursor(8, 1);
        lcd.print("LED: "); // 3 espaços
        lcd.setCursor(12, 1);
        lcd.print(led_value);

        refresh = true;
      } else {
        lcd.setCursor(12, 1);
        lcd.print("   ");
        lcd.setCursor(12, 1);
        lcd.print(led_value);
        lcd.setCursor(4, 1);
        lcd.print("    ");
        lcd.setCursor(4, 1);
        lcd.print(adr_ldr);
        lcd.setCursor(5, 0);
        lcd.print("    ");
        lcd.setCursor(5, 0);
        lcd.print(analogRead(PIN_ERROR));
      }
      break;

    case btnDown:
      if (!refresh) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HOME");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print("LED ");
        lcd.setCursor(6, 1);
        lcd.write(byte(2));
        lcd.print("ALL  ");
        lcd.setCursor(12, 1);
        lcd.print("LDR");
        lcd.write(byte(1));

        refresh = true;
      }
      break;

    case btnLeft:
      if (!refresh) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("LED: ");
        lcd.setCursor(4, 0);
        lcd.print(map(led_value, 0, 255, 0, 100));
        lcd.setCursor(7, 0);
        lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.write(byte(3));
        lcd.print("HOME ");
        lcd.setCursor(6, 1);
        lcd.write(byte(2));
        lcd.print("ALL  ");
        lcd.setCursor(12, 1);
        lcd.print("LDR");
        lcd.write(byte(1));

        refresh = true;
      } else {
        lcd.setCursor(4, 0);
        lcd.print("   ");
        lcd.setCursor(4, 0);
        lcd.print(map(led_value, 0, 255, 0, 100));
      }
      break;

    case btnSelect:
      break;
    }
  }
};
} // namespace UI

#endif // UI_H