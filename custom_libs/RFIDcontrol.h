#ifndef __RFIDCONTROL__
#define __RFIDCONTROL__

#include <MFRC522.h>

#include "ReaderCount.h"


void initialiseReaders(MFRC522 (&readers)[NR_Readers], byte pins[], byte reset, int nr_readers);

void rfid_read(MFRC522 (&readers)[NR_Readers]);

void dumpUID(MFRC522::Uid uid);

bool isMasterCard(MFRC522::Uid uid);

void addRemove_tag(MFRC522::Uid uid);

bool uid_equal(byte id1[], MFRC522::Uid id2);

int findIndexOfTag(MFRC522::Uid uid);

void removeEmptyTags(int startIndex);

#endif