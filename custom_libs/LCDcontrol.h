#ifndef __LCDCONTROL__
#define __LCDCONTROL__

#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

void initialiseLCD(LiquidCrystal_I2C &lcd);
void writeToLCD(const char* str);
void writeToLCD(const char* str, int x, int y);
void writeToLCD(MFRC522::Uid uid);
void clearLCD();
void clearLine(int line);

#endif