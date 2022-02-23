#include <MFRC522.h>

#include "custom_libs/ReaderCount.h"

#define Max_Tags 100

const byte masterCard[] = {0xC3, 0x66, 0xCB, 0x18};
byte tags[Max_Tags][4];
bool access = false;
unsigned int nr_tags = 0;

// forward declaration
void dumpUID(MFRC522::Uid uid);
bool isMasterCard(MFRC522::Uid uid);
void addRemove_tag(MFRC522::Uid uid);
bool tagIsKnown(MFRC522::Uid uid);
bool uid_equal(MFRC522::Uid id1, MFRC522::Uid id2);
int findIndexOfTag(MFRC522::UID uid);
void removeEmptyTags();


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

void addRemove_tag(MFRC522::Uid uid) {
    if (tagIsKnown) {
        // remove the tag
        int index = findIndexOfTag(uid);
        tags[index] = {};
        removeEmptyTags(index);
        delete index;
    }
    else {
        // add the tag
        tags[nr_tags - 1] = uid.uidByte;
        nr_tags++;
    }
}

bool tagIsKnown(MFRC522::Uid uid) {
    for (byte[] tag : tags) {
        if (uid_equal(tag, uid)) {
            return true;
        }
    }
    return false;
}

bool uid_equal(byte[] id1, MFRC522::Uid id2) {
    if (id1.size == id2.size) {
        for (int i = 0; i < id1.size; i++) {
            if (int(id1[i]) != int(id2.uidByte[i])) {
                return false;
            }
        }

        return true;
    }
    else {
        return false;
    }
}

int findIndexOfTag(MFRC522::Uid uid) {
    for (int i = 0; i < nr_tags; i++) {
        if (uid_equal(tags[i]), uid) {
            return i;
        }
    }

    return -1;
}

void removeEmptyTags(int startIndex) {
    for (int i = startIndex; i < Max_Tags - 1; i++) {
        if (tags[i + 1] != {}) {
            tags[i] = tags[i + 1];
            tags[i + 1] = {};
        }
        else {
            return;
        }
    }
}