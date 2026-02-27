#include <SPI.h>
#include <SSD1803A_I2C.h>
#include "pins.h"

SSD1803A_I2C lcd(&SPI);

void DisplayInit(void) {
  pinMode(PIN_LCD_MOSI, OUTPUT);
  pinMode(PIN_LCD_SCK, OUTPUT);
  pinMode(PIN_LCD_MISO, INPUT);
  pinMode(PIN_LCD_nRESET, OUTPUT);
  
  SPI.begin(PIN_LCD_SCK, PIN_LCD_MISO, PIN_LCD_MOSI);
  lcd.begin(DOGM204, PIN_LCD_nRESET);
  lcd.home();
  lcd.print("Hello, World!");
}

void DisplayWrite(String buf) {
  lcd.print(buf);
}

void DisplayClear(void) {
  lcd.cls();
  lcd.home(); // TODO: is this needed?
}

void DisplayClearRow(uint8_t row) {
  lcd.clr(row);
}

void DisplayHomeCursor(void) {
  lcd.home();
}

void DisplayMoveCursor(uint8_t row, uint8_t column) {
  lcd.locate(row, column);
}