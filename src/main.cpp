#include <Arduino.h>
#include <LiquidCrystal.h>
#include "UI.h"
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define PIN_LDR A1
#define PIN_ERRO A2
#define PIN_LED 11

#define DELAY 100
#define LUM_ALTA 1000
#define LUM_BAIXA 150
#define TEMPO_TIMEOUT 10000

void printInfo(uint8_t led_value, uint16_t adr_ldr){
  lcd.setCursor(7, 0);
  lcd.print("    "); //4 espaços
  lcd.setCursor(7, 0);
  lcd.print(analogRead(PIN_ERRO));
  lcd.setCursor(4, 1);
  lcd.print("    "); //4 espaços
  lcd.setCursor(4, 1);
  lcd.print(adr_ldr);
  lcd.setCursor(13, 1);
  lcd.print("   "); //3 espaços
  lcd.setCursor(13, 1);
  lcd.print(led_value);
}

uint8_t ldr_to_led(uint16_t value, uint16_t lumbaixa, uint16_t lumalta){
  if(value <= lumbaixa || value >= lumalta)
  return 0;
  else{
    return map(value, lumbaixa, lumalta, 50, 255);
  }
}

void setup(){
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_ERRO, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, Character::leftArrow);
  lcd.createChar(1, Character::rightArrow);
  lcd.createChar(2, Character::upArrow);
  lcd.createChar(3, Character::downArrow);
  lcd.clear();
  lcd.home();
  UI::HUD::draw(lcd);
}

void loop(){
  //Was going to draw the hud every frame, but figured i'd draw it on the setup and then
  //on a 100ms delay update.
  //UI::HUD::draw(lcd);

  uint16_t adr_ldr = analogRead(PIN_LDR);
  uint8_t led_value = ldr_to_led(adr_ldr, LUM_BAIXA, LUM_ALTA);
  static uint32_t ultimo_tempo = millis();
  analogWrite(PIN_LED, led_value);
  uint32_t tempo_atual = millis();

  if(tempo_atual - ultimo_tempo >= DELAY){
    UI::HUD::draw(lcd);
    //printInfo(led_value, adr_ldr);
    ultimo_tempo = tempo_atual;
  }
  static uint32_t lastTimeout = millis();
  uint32_t timeTimeout = millis();
  if(timeTimeout - lastTimeout >= TEMPO_TIMEOUT /*&& botão pressionado*/){
    lcd.noBacklight();
  }
}