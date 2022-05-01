#include <MFRC522.h>

#include "custom_libs/ReaderCount.h"
#include "custom_libs/RFIDcontrol.h"

#define Max_Tags 10

byte masterCard[] = {0xC3, 0x66, 0xCB, 0x18};
byte tags[Max_Tags][4];
bool access = false;
unsigned int nr_tags = 0;


// functions
void initialiseReaders(MFRC522 (&readers)[NR_Readers], byte pins[], byte reset, int nr_readers) {
    // readers will always be a reference to an array
    for (int i = 0; i < nr_readers; i++) {
        readers[i].PCD_Init(pins[i], reset);
        readers[i].PCD_DumpVersionToSerial();
    }
}

void dumpUID(MFRC522::Uid uid) {
    for ( int i=0; i < uid.size; i++ ) {
        Serial.print(int(uid.uidByte[i]), HEX);
        if ( i < uid.size - 1 ) {
            Serial.print(" ");
        }
    }
}

bool isMasterCard(MFRC522::Uid uid) {
    if (!uid_equal(masterCard, uid)) { return false; }

    Serial.println("Master card present!");
    return true;
}

int addRemove_tag(MFRC522::Uid uid) {
    if (tagIsKnown(uid)) {
        // remove the tag
        int index = findIndexOfTag(uid);
        for (int i = 0; i < 4; i++) {
            tags[index][i] = 0;
        }
        nr_tags--;
        // removeEmptyTags(index);
        return -1;      // remove code
    }
    else {
        // add the tag
        for (int i = 0; i < 4; i++) {
            tags[nr_tags][i] = int(uid.uidByte[i]);
        }
        nr_tags++;
        return 1;       // add code
    }
}

bool tagIsKnown(MFRC522::Uid uid) {
    for (int i = 0; i < Max_Tags; i++) {
        if (uid_equal(tags[i], uid)) {
            return true;
        }
    }

    return false;
}

bool uid_equal(byte* id1, MFRC522::Uid id2) {
    for (int i = 0; i < id2.size; i++) {
        if (int(id1[i]) != int(id2.uidByte[i])) {
            return false;
        }
    }

    return true;
}

int findIndexOfTag(MFRC522::Uid uid) {
    for (int i = 0; i < nr_tags; i++) {
        if (uid_equal(tags[i], uid)) {
            return i;
        }
    }

    return -1;
}

void removeEmptyTags(int startIndex) {
    for (int i = startIndex; i < Max_Tags - 1; i++) {
        byte comp[4] = {0, 0, 0, 0};
        if (tags[i + 1] != comp) {
            for (int j = 0; j < 4; j++) {
                tags[i][j] = tags[i + 1][j];
            }
            for (int j = 0; j < 4; j++) {
                tags[i+1][j] = -1;
            }
        }
        else {
            return;
        }
    }
}