#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define PIN_LDR A1
#define PIN_ERRO A2
#define PIN_LED 11

#define DELAY 100
#define LUM_ALTA 1000
#define LUM_BAIXA 150

// Creating custom chars for menu
class Character
{
  public:
    byte leftArrow[] = {
      B00011,
      B00111,
      B01111,
      B11111,
      B11111,
      B01111,
      B00111,
      B00011
    };
    byte rightArrow[] = {
      B11000,
      B11100,
      B11110,
      B11111,
      B11111,
      B11110,
      B11100,
      B11000
    };
    byte upArrow[] = {
      B00000,
      B00000,
      B00100,
      B01110,
      B11111,
      B11111,
      B00000,
      B00000
    };
    byte downArrow[] = {
      B00000,
      B00000,
      B11111,
      B11111,
      B01110,
      B00100,
      B00000,
      B00000
    };
}

void drawHud(){
  lcd.setCursor(0, 0);
  lcd.print("HOME");
  lcd.setCursor(0, 1);
  lcd.print("<LED ");
  lcd.setCursor(6, 1);
  lcd.print("^ALL  ");
  lcd.setCursor(12, 1);
  lcd.print("LDR>");
  /*switch(event){
    case SELECT:

    break;
    case LEFT:

    break;
    case UP:

    break;
    case DOWN:

    break;
    case RIGHT:

    break;
  }*/
}

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
  lcd.createChar(0, leftArrow);
  lcd.createChar(0, rightArrow);
  lcd.createChar(0, upArrow);
  lcd.createChar(0, downArrow);
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("ERRO:  "); //2 espaços
  lcd.setCursor(0, 1);
  lcd.print("LDR:    LED: ");
}

void loop(){
  //drawHud();
  uint16_t adr_ldr = analogRead(PIN_LDR);
  uint8_t led_value = ldr_to_led(adr_ldr, LUM_BAIXA, LUM_ALTA);
  static uint32_t ultimo_tempo = millis();
  analogWrite(PIN_LED, led_value);
  uint32_t tempo_atual = millis();
  if(tempo_atual - ultimo_tempo >= DELAY){
    drawHud();
    //printInfo(led_value, adr_ldr);
    ultimo_tempo = tempo_atual;
  }
}
