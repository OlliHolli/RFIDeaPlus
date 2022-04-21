#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "custom_libs/ReaderCount.h"
#include "custom_libs/LCDcontrol.h"
#include "custom_libs/RFIDcontrol.h"
#include "custom_libs/RFIDea.h"

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

RFIDea::RFIDea() { editormode = false; }

RFIDea::RFIDea(bool _editormode) { editormode = _editormode; }

void RFIDea::toggleEditorMode() { editormode = !editormode; }

void RFIDea::initialise() {
            initialiseReaders(rfid_readers, sda_pins, RST_Pin, NR_Readers);
            initialiseLCD(lcd);
}

void RFIDea::read() {
    byte** arr = rfid_read(rfid_readers);
    Serial.println(arrIsEmpty(arr));
    if (!arrIsEmpty(arr)) {
        for (int i = 0; i < NR_Readers; i++) {
            if (isMasterCard(rfid_readers[i].uid)) {
                toggleEditorMode();
                if (editormode) {
                    Serial.println("Editormode enabled!");
                    Serial.println("Put a tag on a reader to add/remove it.");
                }
                else {
                    Serial.println("Editormode disabled.");
                    Serial.println("Default operation.");
                }
            }
            else {
                if (editormode) {
                    int addRemove = addRemove_tag(rfid_readers[i].uid);
                    Serial.print("Chip ");
                    dumpUID(rfid_readers[i].uid);
                    Serial.print("was ");
                    if (addRemove == 1) {
                        Serial.println("added.");
                    }
                    else {
                        Serial.println("removed.");
                    }
                }
                else {
                    // Default behaviour
                    if (tagIsKnown(rfid_readers[i].uid)) {
                        Serial.println(String("Access granted on Reader " + (i + 1)));
                    }
                    else {
                        Serial.println(String("Access denied on Reader " + (i + 1)));
                    }
                }
            }
        }
    }
}

bool RFIDea::arrIsEmpty(byte** arr) {
    for (int i = 0; i < sizeof(arr); i++) {
        for (int j = 0; j < 4; j++) {
            if (int(arr[i][j]) != 0) { return false; }
        }
    }

    return true;
}