#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C _lcd;

void initialiseLCD(LiquidCrystal_I2C &lcd) {
    _lcd = lcd;
    _lcd.init();
    delay(1000);
    _lcd.backlight();
}

void writeToLCD(uint8_t text) {
    _lcd.write(text);
}

void writeTOLCD(uint8_t text, int x, int y) {       // uint8_t is used as string
    _lcd.setCursor(x, y);
    _lcd.write(text);
}

void clearLCD() { _lcd.clear(); }