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