// Main program for the RFIDea+ project
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "custom_libs\LCDcontrol.h"
#include "custom_libs\RFIDcontrol.h"
#include "custom_libs\ReaderCount.h"

// RFID reader
#define RST_Pin 5

#define SDA_Pin_1 53
#define SDA_Pin_2 25

byte sda_pins[NR_Readers] = {SDA_Pin_1, SDA_Pin_2};
MFRC522 rfid_readers[NR_Readers];       // NR_Readers is defined in ReaderCount.h

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// functions
void setup() {
    Serial.begin(115200);
    SPI.begin();

    initialiseReaders(rfid_readers, sda_pins, RST_Pin, NR_Readers);
    initialiseLCD(lcd);
    delay(1000);        // safe delay to make sure initialisation is finished before usage
}

void loop() {
    rfid_read(rfid_readers);
}
