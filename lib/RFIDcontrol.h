#ifndef __RFIDCONTROL__
#define __RFIDCONTROL__

#include <MFRC522.h>        // for some reason this must stay here

void initialiseReaders(MFRC522 &readers);

#endif