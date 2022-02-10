// Main programm for the RFIDea+ project
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "MaltesTollerKot.h"


// RFID reader
#define RST_Pin 5
#define SDA_Pin_1 53

#define SDA_Pin_2 25

MFRC522 rfid_reader_1(SDA_Pin_1, RST_Pin);
MFRC522 rfid_reader_2(SDA_Pin_2, RST_Pin);

byte masterCard[] = {0xC3, 0x66, 0xCB, 0x18};


bool access = false;

void setup() {
    Serial.begin(115200);
    SPI.begin();
    rfid_reader_1.PCD_Init();
    rfid_reader_2.PCD_Init();
    delay(1000);
    rfid_reader_1.PCD_DumpVersionToSerial();
    rfid_reader_2.PCD_DumpVersionToSerial();
}

void loop() {
    if (rfid_reader_1.PICC_IsNewCardPresent()) {
        Serial.println("There is a card on reader 1");
        if ( rfid_reader_1.PICC_ReadCardSerial() ) {
            if ( isMasterCard(rfid_reader_1.uid) ){
                Serial.println("Yes Master?");
            }
            else {
                Serial.print("Chip ID: ");
                dumpUID(rfid_reader_1.uid);
                Serial.println();
            }
            delay(1000);
        }
    }

    if (rfid_reader_2.PICC_IsNewCardPresent()) {
        Serial.println("There is a card on reader 2");
        if ( rfid_reader_2.PICC_ReadCardSerial() ) {
            Serial.print("Chip ID: ");
            dumpUID(rfid_reader_2.uid);
            Serial.println();
            delay(1000);
        }
    }
}

void dumpUID(MFRC522::Uid uid) {
    for ( int i=0; i < uid.size; i++ ) {
        Serial.print(uid.uidByte[i], HEX);
        if ( i < uid.size - 1 ) {
            Serial.print(" ");
        }
    }
}

bool isMasterCard(MFRC522::Uid uid) {
    for (int i = 0; i < uid.size; i++) {
        if (int(uid.uidByte[i]) != int(masterCard[i])) {
            return false;
        }
    }

    Serial.println("Master card present");
    return true;
}