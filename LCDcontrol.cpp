#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <Wire.h>

LiquidCrystal_I2C _lcd(0,0,0);      // must be initialised as no default constructor exists

void clearLCD();

void initialiseLCD(LiquidCrystal_I2C (&lcd)) {
    _lcd = lcd;
    _lcd.init();
    _lcd.backlight();
}

void writeToLCD(const char* str) {
    clearLCD();
    _lcd.setCursor(0, 0);
    _lcd.print(str);
}

void writeToLCD(const char* str, int x, int y) {
    // x: character position x/16
    // y: line y/2
    _lcd.setCursor(x, y);
    _lcd.print(str);
}

void writeToLCD(MFRC522::Uid uid) {
    writeToLCD("ID: ", 0, 1);

    for ( int i=0; i < uid.size; i++ ) {
        _lcd.print(int(uid.uidByte[i]), HEX);
        if ( i < uid.size - 1 ) {
            _lcd.print(" ");
        }
    }
}

void clearLCD() { _lcd.clear(); }

void clearLine(int line) {
    // line should not be bigger than 2
    _lcd.setCursor(0, line);
    _lcd.print("                ");
}