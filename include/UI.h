#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <LiquidCrystal.h>

// Creating custom characters for the UI
namespace Character
{
  byte leftArrow[8] = {B00000, B00010, B00110, B01110, B01110, B00110, B00010, B00000};
  byte rightArrow[8] = {B00000, B01000, B01100, B01110, B01110, B01100, B01000, B00000};
  byte upArrow[8] = {B00000, B00000, B00100, B01110, B11111, B11111, B00000, B00000};
  byte downArrow[8] = {B00000, B00000, B11111, B11111, B01110, B00100, B00000, B00000};
}

namespace UI{
class HUD{
    public:
    static void draw(LiquidCrystal&lcd){
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
        }
    };
} // namespace UI

#endif // UI_H