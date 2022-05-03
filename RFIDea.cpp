#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "custom_libs/ReaderCount.h"
#include "custom_libs/LCDcontrol.h"
#include "custom_libs/RFIDcontrol.h"
#include "custom_libs/RFIDea.h"

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);     // connect to SDA(20), SCL(21) and 5V on Arduino Mega

RFIDea::RFIDea() {
    editormode = false;
    editorMessage = false;
    mastercardReader = -1;
}

RFIDea::RFIDea(bool _editormode) { editormode = _editormode; }

void RFIDea::toggleEditorMode() {
    writeToLCD("Bitte warten...");
    Serial.println("Please wait...");
    delay(1000);        // give me a chance to remove the tag.

    editormode = !editormode;
    editorMessage = editormode;

    if (editormode) {
        writeToLCD("Editormodus ein!");
        writeToLCD("Karte auflegen.", 0, 1);
        Serial.println("Editormode enabled!");
        Serial.println("Put a tag on the same reader to add/remove it.");
    }
    else {
        writeToLCD("Editormodus aus!");
        Serial.println("Editormode disabled.");
        Serial.println("Default operation.");

        delay(2000);
        clearLCD();
        writeToLCD("Guten Tag!", 3, 0);
    }
}

void RFIDea::initialise() {
    initialiseReaders(rfid_readers, sda_pins, RST_Pin, NR_Readers);
    initialiseLCD(lcd);
    delay(1000);
    clearLCD();
    writeToLCD("Guten Tag!", 3, 0);
}

void RFIDea::read() {
    for (int i = 0; i < NR_Readers; i++) {
        if (editormode && editorMessage) {
            writeToLCD("Editormodus ein!");
            writeToLCD("Karte auflegen.", 0, 1);
            editorMessage = false;
        }

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
                        writeToLCD("UID hinzugefügt");
                        Serial.println("added.");
                    }
                    else {
                        writeToLCD("UID entfernt");
                        Serial.println("removed.");
                    }

                    writeToLCD(rfid_readers[i].uid);
                    delay(2000);

                    mastercardReader = -1;
                    toggleEditorMode();
                }
                else {
                    // Default behaviour
                    if (tagIsKnown(rfid_readers[i].uid)) {
                        Serial.print("Access granted on reader ");
                        Serial.println(i + 1);

                        writeToLCD("Zugang gewährt.");
                        writeToLCD("Willkommen!", 0, 1);
                    }
                    else {
                        Serial.print("Access denied on reader ");
                        Serial.println(i + 1);

                        writeToLCD("Zugang verwehrt.");
                        writeToLCD("Erneut versuchen", 0, 1);
                    }

                    delay(3000);
                    clearLCD();
                    writeToLCD("Guten Tag!", 3, 0);

                    editorMessage = editormode;     // reprint editormessage when editormode is still enabled
                }
            }

            rfid_readers[i].PICC_HaltA();
            rfid_readers[i].PCD_StopCrypto1();

            Serial.println();
        }
    }
}