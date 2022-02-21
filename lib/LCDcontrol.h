#ifndef __LCDCONTROL__
#define __LCDCONTROL__

#include <LiquidCrystal_I2C.h>

void initialiseLCD(LiquidCrystal_I2C &lcd);
void writeToLCD(uint8_t text);
void writeTOLCD(uint8_t text, int x, int y);
void clearLCD();

#endif