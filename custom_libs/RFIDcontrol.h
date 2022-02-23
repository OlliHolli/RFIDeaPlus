#ifndef __RFIDCONTROL__
#define __RFIDCONTROL__

#include <MFRC522.h>

#include "ReaderCount.h"


void initialiseReaders(MFRC522 (&readers)[NR_Readers], byte pins[], byte reset, int nr_readers);

void rfid_read(MFRC522 (&readers)[NR_Readers]);

void dumpUID(MFRC522::Uid uid);

bool isMasterCard(MFRC522::Uid uid);

#endif