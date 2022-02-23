#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "custom_libs/ReaderCount.h"
#include "custom_libs/LCDcontrol.h"
#include "custom_libs/RFIDcontrol.h"


// RFID reader
#define RST_Pin 5

#define SDA_Pin_1 53
#define SDA_Pin_2 25

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);


class State {
    public:
        virtual void toggleEditorMode();
};

class EditorMode : public State {
    public:
        EditorMode(RFIDea &_rfidea) { rfidea = _rfidea; }

        void toggleEditorMode() { rfidea.setState(Default(rfidea)); }

    private:
        RFIDea rfidea;
};

class Default : public State {
    public:
        Default(RFIDea &_rfidea) { rfidea = _rfidea; }

        void toggleEditorMode() { rfidea.setState(EditorMode(rfidea)); }

    private:
        RFIDea rfidea;
};

class RFIDea {
    public:
        RFIDea() { setState(Default(*this)); }

        void setState(State newState) { currentState = newState; }

        void toggleEditorMode() { currentState.toggleEditorMode(); }

        void initialise() {
            initialiseReaders(rfid_readers, sda_pins, RST_Pin, NR_Readers);
            initialiseLCD(lcd);
        }

    private:
        State currentState;
        byte sda_pins[NR_Readers] = {SDA_Pin_1, SDA_Pin_2};
        MFRC522 rfid_readers[NR_Readers];       // NR_Readers is defined in ReaderCount.h
};