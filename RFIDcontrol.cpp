#include <MFRC522.h>

#include "custom_libs/ReaderCount.h"

const byte masterCard[] = {0xC3, 0x66, 0xCB, 0x18};
byte tags[][4] = {};
bool access = false;

// forward declaration
void dumpUID(MFRC522::Uid uid);
bool isMasterCard(MFRC522::Uid uid);


// functions
void initialiseReaders(MFRC522 (&readers)[NR_Readers], byte pins[], byte reset, int nr_readers) {
    // readers will always be a reference to an array
    for (int i = 0; i < nr_readers; i++) {
        readers[i].PCD_Init(pins[i], reset);
    }
}

void rfid_read(MFRC522 (&readers)[NR_Readers]) {
    for (int i = 0; i < NR_Readers; i++) {
        if (readers[i].PICC_IsNewCardPresent()) {
            Serial.println("There is a card on reader " + (i + 1));
            if (readers[i].PICC_ReadCardSerial()) {
                if (isMasterCard(readers[i].uid)) {
                    Serial.println("Yes Master?");
                }
                else {
                    Serial.print("Chip ID: ");
                    dumpUID(readers[i].uid);
                    Serial.println();
                }
            }
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