#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "custom_libs/ReaderCount.h"
#include "custom_libs/LCDcontrol.h"
#include "custom_libs/RFIDcontrol.h"
#include "custom_libs/RFIDea.h"

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

RFIDea::RFIDea() {
    editormode = false;
    mastercardReader = -1;
}

RFIDea::RFIDea(bool _editormode) { editormode = _editormode; }

void RFIDea::toggleEditorMode() {
    Serial.println("Please wait...");
    delay(1000);        // give me a chance to remove the tag.

    editormode = !editormode;

    if (editormode) {
        Serial.println("Editormode enabled!");
        Serial.println("Put a tag on the same reader to add/remove it.");
    }
    else {
        Serial.println("Editormode disabled.");
        Serial.println("Default operation.");
    }
}

void RFIDea::initialise() {
    initialiseReaders(rfid_readers, sda_pins, RST_Pin, NR_Readers);
    // initialiseLCD(lcd);
}

void RFIDea::read() {
    for (int i = 0; i < NR_Readers; i++) {
        if(rfid_readers[i].PICC_IsNewCardPresent() && rfid_readers[i].PICC_ReadCardSerial()) {
            Serial.print("There is a card on reader ");
            Serial.println(i + 1);

            Serial.print("UID: ");
            dumpUID(rfid_readers[i].uid);
            Serial.println();

            if (isMasterCard(rfid_readers[i].uid)) {
                mastercardReader = i;
                toggleEditorMode();
            }
            else {
                if (editormode && mastercardReader == i) {
                    int addRemove = addRemove_tag(rfid_readers[i].uid);
                    Serial.print("Chip ");
                    dumpUID(rfid_readers[i].uid);
                    Serial.print(" was ");

                    if (addRemove == 1) {
                        Serial.println("added.");
                    }
                    else {
                        Serial.println("removed.");
                    }

                    mastercardReader = -1;
                    toggleEditorMode();
                }
                else {
                    // Default behaviour
                    if (tagIsKnown(rfid_readers[i].uid)) {
                        Serial.print("Access granted on reader ");
                        Serial.println(i + 1);
                    }
                    else {
                        Serial.print("Access denied on reader ");
                        Serial.println(i + 1);
                    }
                }
            }

            rfid_readers[i].PICC_HaltA();
            rfid_readers[i].PCD_StopCrypto1();

            Serial.println();
        }
    }
}