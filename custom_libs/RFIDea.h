#ifndef __RFIDEA_CLASS__
#define __RFIDEA_CLASS__

#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include "ReaderCount.h"


// RFID reader
#define RST_Pin 5
/*
!!! Required PIN Layout - Cannot be changed !!!
SCK - 52
MOSI - 51
MISO - 50
*/
#define SDA_Pin_1 53
#define SDA_Pin_2 25


class RFIDea {
    public:
        RFIDea();

        RFIDea(bool _editormode);

        void toggleEditorMode();

        void initialise();

        void read();

    private:
        int mastercardReader;
        bool editormode;
        bool editorMessage;
        byte sda_pins[NR_Readers] = {SDA_Pin_1, SDA_Pin_2};
        MFRC522 rfid_readers[NR_Readers];       // NR_Readers is defined in ReaderCount.h
};

#endif